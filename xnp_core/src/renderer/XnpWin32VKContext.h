#pragma once

#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <utility>

#include "Shell.h"
#include "RmlUi_Renderer_VK.h"
#include "RmlUi_Include_Windows.h"
#include "RmlUi_Platform_Win32.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger/Debugger.h>
#include <future>
#include <QThread>

struct Win32VkEvent
{
    UINT message;
    WPARAM w_param;
    LPARAM l_param;

    Win32VkEvent(UINT msg, WPARAM wp, LPARAM lp)
            : message(msg), w_param(wp), l_param(lp) {}
};

class XnpWin32VKContext : public QObject
{
Q_OBJECT
protected:
    // Declare pointers to the DPI aware Windows API functions.
    using ProcSetProcessDpiAwarenessContext = BOOL(WINAPI *)(HANDLE value);
    using ProcGetDpiForWindow = UINT(WINAPI *)(HWND hwnd);
    using ProcAdjustWindowRectExForDpi = BOOL(WINAPI *)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle,UINT dpi);
    bool has_dpi_support = false;
    ProcSetProcessDpiAwarenessContext procSetProcessDpiAwarenessContext = nullptr;
    ProcGetDpiForWindow procGetDpiForWindow = nullptr;
    ProcAdjustWindowRectExForDpi procAdjustWindowRectExForDpi = nullptr;

    // thread define
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<Win32VkEvent> eventQueue;
    std::atomic<bool> running{};

    // Backend define
    SystemInterface_Win32 system_interface;
    RenderInterface_VK render_interface;
    TextInputMethodEditor_Win32 text_input_method_editor;

    bool context_dimensions_dirty = true;
    Rml::Vector2i window_dimensions;
    Rml::Context *context = nullptr;

    HWND window_handle = nullptr;
    std::string window_title;
    int window_width;
    int window_height;

    static std::once_flag initFlag;
    static std::atomic<int> ref_count;  // 静态引用计数器
    static std::atomic<HWND> hwnd;

public:
    XnpWin32VKContext(HWND win_hwnd, std::string title, int width, int height)
            : window_handle(win_hwnd),
              window_title(std::move(title)),
              window_width(width),
              window_height(height)
    {
        window_dimensions.x = window_width;
        window_dimensions.y = window_height;
        // 线程安全，只执行一次
        std::call_once(initFlag, [this]() {
            Rml::Initialise();
            Rml::SetTextInputHandler(&text_input_method_editor);
            Shell::Initialize();
            Shell::LoadFonts();
        });
        ++ref_count;
        Setting();
    }

    ~XnpWin32VKContext() override
    {
        if (--ref_count == 0) {
            Rml::Shutdown();
            if (Rml::GetTextInputHandler() == &text_input_method_editor)
                Rml::SetTextInputHandler(nullptr);
            render_interface.Shutdown();
            Shell::Shutdown();
        }
    }

    void ActivateKeyboard()
    {
        return system_interface.ActivateKeyboard();
    }

    void Setting()
    {
        InitializeDpiSupport();
        Rml::Vector<const char *> extensions;
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        auto CreateVulkanSurface = [](VkInstance instance, VkSurfaceKHR *out_surface) {
            VkWin32SurfaceCreateInfoKHR info = {};
            info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            info.hinstance = GetModuleHandle(nullptr);
            info.hwnd = hwnd.load();
            VkResult status = vkCreateWin32SurfaceKHR(instance, &info, nullptr, out_surface);
            bool result = (status == VK_SUCCESS);
            return result;
        };
        hwnd.store(this->window_handle);
        if (!render_interface.Initialize(std::move(extensions), CreateVulkanSurface)) {
            throw std::runtime_error("Failed to initialize Vulkan render interface");
        }
        Rml::SetSystemInterface(&system_interface);
        //auto sharedThis = std::shared_ptr<XnpWin32VKContext>(this, [](XnpWin32VKContext*) {});  // 创建不管理生命周期的 shared_ptr
        system_interface.SetWindow(window_handle);
        system_interface.SetParent(this);  // 设置 parent
        render_interface.SetViewport(window_width, window_height);
    }

    virtual void Init() = 0;

    void Run()
    {
        Init();
        Loop();
    }

    void Loop()
    {
        running = true;
        std::unique_lock<std::mutex> lock(mutex);
        while (running) {
            // cv.wait_for(lock, std::chrono::milliseconds(16),[=] {
            //     bool rvl = false;
            //     while (!eventQueue.empty()) {
            //         Win32VkEvent event = eventQueue.front();
            //         eventQueue.pop();
            //         ProcessEvents(event.message, event.w_param, event.l_param);
            //         rvl = true;
            //     }
            //     return rvl;
            // });
            cv.wait(lock,[=] {
                bool rvl = false;
                while (!eventQueue.empty()) {
                    Win32VkEvent event = eventQueue.front();
                    eventQueue.pop();
                    ProcessEvents(event.message, event.w_param, event.l_param);
                    rvl = true;
                }
                return rvl;
            });
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // 防止事件频繁,卡顿
            context->Update();
            render_interface.BeginFrame();
            context->Render();
            render_interface.EndFrame();
        }
        Rml::ReleaseTextures(&render_interface);
        Rml::RemoveContext(window_title);
    }

    void DispatchEvent(UINT message, WPARAM w_param, LPARAM l_param);

    bool ProcessEvents(UINT message, WPARAM w_param, LPARAM l_param);

    bool ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority){
        if (!context)
            return true;
        // Result should return true to allow the event to propagate to the next handler.
        bool result = false;
        // This function is intended to be called twice by the backend, before and after submitting the key event to the context. This way we can
        // intercept shortcuts that should take priority over the context, and then handle any shortcuts of lower priority if the context did not
        // intercept it.
        if (priority)
        {
            // Priority shortcuts are handled before submitting the key to the context.
            // Toggle debugger and set dp-ratio using Ctrl +/-/0 keys.
            if (key == Rml::Input::KI_F8)
            {
                Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
            }
            else if (key == Rml::Input::KI_0 && key_modifier & Rml::Input::KM_CTRL)
            {
                context->SetDensityIndependentPixelRatio(native_dp_ratio);
            }
            else if (key == Rml::Input::KI_1 && key_modifier & Rml::Input::KM_CTRL)
            {
                context->SetDensityIndependentPixelRatio(1.f);
            }
            else if ((key == Rml::Input::KI_OEM_MINUS || key == Rml::Input::KI_SUBTRACT) && key_modifier & Rml::Input::KM_CTRL)
            {
                const float new_dp_ratio = Rml::Math::Max(context->GetDensityIndependentPixelRatio() / 1.2f, 0.5f);
                context->SetDensityIndependentPixelRatio(new_dp_ratio);
            }
            else if ((key == Rml::Input::KI_OEM_PLUS || key == Rml::Input::KI_ADD) && key_modifier & Rml::Input::KM_CTRL)
            {
                const float new_dp_ratio = Rml::Math::Min(context->GetDensityIndependentPixelRatio() * 1.2f, 2.5f);
                context->SetDensityIndependentPixelRatio(new_dp_ratio);
            }
            else
            {
                // Propagate the key down event to the context.
                result = true;
            }
        }
        else
        {
            // We arrive here when no priority keys are detected and the key was not consumed by the context. Check for shortcuts of lower priority.
            if (key == Rml::Input::KI_R && key_modifier & Rml::Input::KM_CTRL)
            {
                for (int i = 0; i < context->GetNumDocuments(); i++)
                {
                    Rml::ElementDocument* document = context->GetDocument(i);
                    const Rml::String& src = document->GetSourceURL();
                    if (src.size() > 4 && src.substr(src.size() - 4) == ".rml")
                    {
                        document->ReloadStyleSheet();
                    }
                }
            }
            else
            {
                result = true;
            }
        }

        return result;
    }

    LRESULT CALLBACK WindowProcedureHandler(UINT message, WPARAM w_param, LPARAM l_param);

    float GetDensityIndependentPixelRatio();

    void InitializeDpiSupport();
signals:
    void ActivateImm();
};




