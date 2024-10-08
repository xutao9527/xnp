#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <utility>


#include "Shell.h"
#include "RmlUi_Backend.h"
#include "RmlUi_Renderer_VK.h"
#include "RmlUi_Include_Windows.h"
#include "RmlUi_Platform_Win32.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger/Debugger.h>

struct Win32VkEvent
{
    UINT message;
    WPARAM w_param;
    LPARAM l_param;
    Win32VkEvent(UINT msg, WPARAM wp, LPARAM lp)
            : message(msg), w_param(wp), l_param(lp) {}
};

class XnpWin32VKContext : public std::enable_shared_from_this<XnpWin32VKContext>
{
protected:
    // Declare pointers to the DPI aware Windows API functions.
    using ProcSetProcessDpiAwarenessContext = BOOL(WINAPI *)(HANDLE value);
    using ProcGetDpiForWindow = UINT(WINAPI *)(HWND hwnd);
    using ProcAdjustWindowRectExForDpi = BOOL(WINAPI *)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi);
    bool has_dpi_support = false;
    ProcSetProcessDpiAwarenessContext procSetProcessDpiAwarenessContext = nullptr;
    ProcGetDpiForWindow procGetDpiForWindow = nullptr;
    ProcAdjustWindowRectExForDpi procAdjustWindowRectExForDpi = nullptr;

    // thread define
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<Win32VkEvent> eventQueue;
    std::atomic<bool> running{};
    static std::once_flag initFlag;

    // Backend define
    SystemInterface_Win32 system_interface;
    RenderInterface_VK render_interface;
    TextInputMethodEditor_Win32 text_input_method_editor;

    bool context_dimensions_dirty = true;
    Rml::Vector2i window_dimensions;
    Rml::Context *context = nullptr;
    KeyDownCallback key_down_callback = nullptr;

    HWND window_handle = nullptr;
    std::wstring window_title;
    int window_width;
    int window_height;
    static std::atomic<HWND> hwnd;

public:
    XnpWin32VKContext(HWND win_hwnd, std::wstring title, int width, int height)
            : window_handle(win_hwnd),
              window_title(std::move(title)),
              window_width(width),
              window_height(height)
    {
        // 线程安全，只执行一次
        std::call_once(initFlag, [this]() {
            Rml::Initialise();
            Rml::SetTextInputHandler(&text_input_method_editor);
            Shell::Initialize();
            Shell::LoadFonts();
        });
        Setting();
    }

    ~XnpWin32VKContext()
    {
        Rml::Shutdown();
        if (Rml::GetTextInputHandler() == &text_input_method_editor)
            Rml::SetTextInputHandler(nullptr);
        render_interface.Shutdown();
        Shell::Shutdown();
    }

    void Setting(){
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
            RMLUI_VK_ASSERTMSG(result, "Failed to create Win32 Vulkan surface");
            return result;
        };
        hwnd.store(this->window_handle);
        if (!render_interface.Initialize(std::move(extensions), CreateVulkanSurface)) {
            wxLogError("Failed to initialize Vulkan render interface");
            throw std::runtime_error("Failed to initialize Vulkan render interface");
        }
        //Rml::SetSystemInterface(&system_interface);
        //Rml::SetRenderInterface(&render_interface);
        system_interface.SetWindow(window_handle);
        render_interface.SetViewport(window_width, window_height);
    }
    virtual void Init() = 0;


    void Run()
    {
        auto self = shared_from_this();
        Init();
        std::thread loopThread([=] {
            self->Loop();
        });
        loopThread.detach();
    }

    void Loop()
    {
        running = true;
        std::unique_lock<std::mutex> lock(mutex);
        while (running) {
            cv.wait(lock, [=] {
                bool rvl = false;
                while (!eventQueue.empty()) {
                    Win32VkEvent event = eventQueue.front();
                    eventQueue.pop();
                    //wxLogMessage(L"处理事件:%d", event.message);
                    ProcessEvents(event.message, event.w_param, event.l_param);
                    rvl = true;
                }
                return rvl;
            });
            context->Update();
            render_interface.BeginFrame();
            context->Render();
            render_interface.EndFrame();
        }
        //Rml::ReleaseTextures(&render_interface);
        Rml::RemoveContext(Shell::ConvertToString(window_title));
    }

    void DispatchEvent(UINT message, WPARAM w_param, LPARAM l_param);

    bool ProcessEvents(UINT message, WPARAM w_param, LPARAM l_param);

    LRESULT CALLBACK WindowProcedureHandler(UINT message, WPARAM w_param, LPARAM l_param);

    float GetDensityIndependentPixelRatio();

    void InitializeDpiSupport();
};




