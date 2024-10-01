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



#include <RmlUi/Core.h>
#include "Shell.h"
#include "RmlUi_Backend.h"
#include "RmlUi_Renderer_VK.h"
#include "RmlUi_Include_Windows.h"
#include "RmlUi_Platform_Win32.h"




struct Win32VkEvent
{
    UINT message;
    WPARAM w_param;
    LPARAM l_param;

    Win32VkEvent(UINT msg, WPARAM wp, LPARAM lp)
            : message(msg), w_param(wp), l_param(lp) {}
};

static bool CreateVulkanSurface(VkInstance instance, VkSurfaceKHR* out_surface)
{
    VkWin32SurfaceCreateInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hinstance = GetModuleHandle(nullptr);
    //info.hwnd = window_handle;

    VkResult status = vkCreateWin32SurfaceKHR(instance, &info, nullptr, out_surface);

    bool result = (status == VK_SUCCESS);
    RMLUI_VK_ASSERTMSG(result, "Failed to create Win32 Vulkan surface");
    return result;
}

class XnpWin32VKContext : public std::enable_shared_from_this<XnpWin32VKContext>
{
private:
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
    static std::once_flag initFlag;

    // Backend define
    SystemInterface_Win32 system_interface;
    RenderInterface_VK render_interface;
    TextInputMethodEditor_Win32 text_input_method_editor;

    HINSTANCE instance_handle = nullptr;
    std::wstring instance_name;

    bool context_dimensions_dirty = true;
    Rml::Vector2i window_dimensions;
    Rml::Context* context = nullptr;
    KeyDownCallback key_down_callback = nullptr;
    HWND window_handle = nullptr;
    std::string window_title;
    int window_width;
    int window_height;
    static std::atomic<HWND> hwnd;
public:
    XnpWin32VKContext(HWND hwnd, std::string title, int width, int height)
            : window_handle(hwnd),
              window_title(std::move(title)),
              window_width(width),
              window_height(height)
    {
        // 线程安全，只执行一次
        std::call_once(initFlag, []() {
            Rml::Initialise();
            Shell::Initialize();
            Shell::LoadFonts();
        });
        Init();
    }

    ~XnpWin32VKContext()
    {
        Rml::Shutdown();
        Rml::SetTextInputHandler(nullptr);
        render_interface.Shutdown();
    }

    void Init()
    {
        InitializeDpiSupport();
        Rml::Vector<const char*> extensions;
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

        auto CreateVulkanSurface = [](VkInstance instance, VkSurfaceKHR* out_surface) {
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
        if (!render_interface.Initialize(std::move(extensions), CreateVulkanSurface)){
            wxLogError("Failed to initialize Vulkan render interface");
        }
        system_interface.SetWindow(window_handle);
        render_interface.SetViewport(window_width, window_height);
        Rml::SetTextInputHandler(&text_input_method_editor);
        Rml::SetSystemInterface(&system_interface);
        Rml::SetRenderInterface(&render_interface);
        context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
        key_down_callback =  &Shell::ProcessKeyDownShortcuts;
        Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml");
        document->Show();
    }

    void Run()
    {
        auto self = shared_from_this();
        std::thread loopThread([=]{
            self->Loop();
        });
        loopThread.detach();
    }

    void PushEvent(UINT message, WPARAM w_param, LPARAM l_param)
    {
        wxLogMessage(L"添加事件:%d", message);
        eventQueue.emplace(message, w_param, l_param);
        cv.notify_all();
    }

    void Loop()
    {
        running = true;

        std::unique_lock<std::mutex> lock(mutex);
        while (running) {
            cv.wait(lock, [=] {
                bool rvl = false;
                while (!eventQueue.empty()){
                    Win32VkEvent event = eventQueue.front();
                    eventQueue.pop();
                    wxLogMessage(L"处理事件:%d", event.message);
                    ProcessEvents(window_handle, event.message, event.w_param, event.l_param);
                    rvl = true;
                }
                return rvl;
            });
            context->Update();
            // Backend::BeginFrame();
            render_interface.BeginFrame();
            context->Render();
            render_interface.EndFrame();
        }
        Rml::ReleaseTextures(&render_interface);
        Rml::RemoveContext("main");
    }

    bool ProcessEvents(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param){
        if (context_dimensions_dirty)
        {
            context_dimensions_dirty = false;
            const float dp_ratio = GetDensityIndependentPixelRatio(window_handle);
            context->SetDimensions(window_dimensions);
            context->SetDensityIndependentPixelRatio(dp_ratio);
        }
        WindowProcedureHandler(window_handle,message,w_param,l_param);
        while (!render_interface.IsSwapchainValid()){
            if (!render_interface.IsSwapchainValid())
                render_interface.RecreateSwapchain();
        }
        return running;
    }

    LRESULT CALLBACK WindowProcedureHandler(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
    {
        switch (message)
        {
            case WM_CLOSE:
            {
                running = false;
                return 0;
            }

            case WM_SIZE:
            {
                const int width = LOWORD(l_param);
                const int height = HIWORD(l_param);
                window_dimensions.x = width;
                window_dimensions.y = height;

                if (context)
                {
                    render_interface.SetViewport(width, height);
                    context->SetDimensions(window_dimensions);
                }
                return 0;
            }

            case WM_DPICHANGED:
            {
                RECT* new_pos = (RECT*)l_param;
                SetWindowPos(window_handle, NULL, new_pos->left, new_pos->top, new_pos->right - new_pos->left, new_pos->bottom - new_pos->top,
                             SWP_NOZORDER | SWP_NOACTIVATE);
                if (context && has_dpi_support)
                    context->SetDensityIndependentPixelRatio(GetDensityIndependentPixelRatio(window_handle));
                return 0;
            }

            case WM_KEYDOWN:
            {
                // Override the default key event callback to add global shortcuts for the samples.
                Rml::Context* context = context;
                 key_down_callback = key_down_callback;

                const Rml::Input::KeyIdentifier rml_key = RmlWin32::ConvertKey((int)w_param);
                const int rml_modifier = RmlWin32::GetKeyModifierState();
                const float native_dp_ratio = GetDensityIndependentPixelRatio(window_handle);

                // See if we have any global shortcuts that take priority over the context.
                if (key_down_callback && !key_down_callback(context, rml_key, rml_modifier, native_dp_ratio, true))
                    return 0;
                // Otherwise, hand the event over to the context by calling the input handler as normal.
                if (!RmlWin32::WindowProcedure(context, text_input_method_editor, window_handle, message, w_param, l_param))
                    return 0;
                // The key was not consumed by the context either, try keyboard shortcuts of lower priority.
                if (key_down_callback && !key_down_callback(context, rml_key, rml_modifier, native_dp_ratio, false))
                    return 0;
                return 0;
            }
            default:
            {
                // Submit it to the platform handler for default input handling.
                if (!RmlWin32::WindowProcedure(context, text_input_method_editor, window_handle, message, w_param, l_param))
                    return 0;
            }
        }
        //return DefWindowProc(window_handle, message, w_param, l_param);
        return 0;
    }

    float GetDensityIndependentPixelRatio(HWND window_handle)
    {
        return float(GetWindowDpi(window_handle)) / float(USER_DEFAULT_SCREEN_DPI);
    }

    UINT GetWindowDpi(HWND window_handle)
    {
        if (has_dpi_support)
        {
            UINT dpi = procGetDpiForWindow(window_handle);
            if (dpi != 0)
                return dpi;
        }
        return USER_DEFAULT_SCREEN_DPI;
    }



    void InitializeDpiSupport()
    {
        // Cast function pointers to void* first for MinGW not to emit errors.
        procSetProcessDpiAwarenessContext = (ProcSetProcessDpiAwarenessContext) (void *) GetProcAddress(
                GetModuleHandle(TEXT("User32.dll")), "SetProcessDpiAwarenessContext");
        procGetDpiForWindow = (ProcGetDpiForWindow) (void *) GetProcAddress(GetModuleHandle(TEXT("User32.dll")),
                                                                            "GetDpiForWindow");
        procAdjustWindowRectExForDpi = (ProcAdjustWindowRectExForDpi) (void *) GetProcAddress(
                GetModuleHandle(TEXT("User32.dll")),
                "AdjustWindowRectExForDpi");

        if (!has_dpi_support && procSetProcessDpiAwarenessContext != nullptr && procGetDpiForWindow != nullptr && procAdjustWindowRectExForDpi != nullptr) {
            // Activate Per Monitor V2.
            if (procSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
                has_dpi_support = true;
        }
    }


};




