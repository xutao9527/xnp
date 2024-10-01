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

#include <Shell.h>
#include <RmlUi/Core/Core.h>
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
    // Arguments set during event processing and nulled otherwise.
    Rml::Context* context = nullptr;
    KeyDownCallback key_down_callback = nullptr;

    HWND window_handle = nullptr;
    std::string window_title;
    int window_width;
    int window_height;
public:
    XnpWin32VKContext(HWND hwnd, std::string title, int width, int height)
            : window_handle(hwnd),
              window_title(std::move(title)),
              window_width(width),
              window_height(height)
    {
        // 线程安全，只执行一次
        std::call_once(initFlag, []() {
            Shell::Initialize();
            Shell::LoadFonts();
        });
        Init();
    }

    void Init()
    {

    }

    void PushEvent(UINT message, WPARAM w_param, LPARAM l_param)
    {
        wxLogMessage(L"添加事件:%d", message);
        eventQueue.emplace(message, w_param, l_param);
        cv.notify_all();
    }

    void Run()
    {
        running = true;
        auto self = shared_from_this();
        std::thread loopThread([=]{
            self->Loop();
        });
        loopThread.detach();
    }

    void Loop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (running) {
            cv.wait(lock, [=] {
                bool rvl = false;
                while (!eventQueue.empty()){
                    Win32VkEvent event = eventQueue.front();
                    eventQueue.pop();
                    wxLogMessage(L"处理事件:%d", event.message);
                    rvl = true;
                }
                return rvl;
            });
        }
    }

    ~XnpWin32VKContext()
    {
        int a = 0;
        wxLogMessage(L"析构%d", a);
    }
};




