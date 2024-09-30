#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <memory>
#include <RmlUi/Core.h>
#include <thread>
#include "RmlUi_Backend.h"
#include "RmlUi_Include_Windows.h"
#include "RmlUi_Platform_Win32.h"
#include "RmlUi_Renderer_VK.h"

struct BackendData {
    SystemInterface_Win32 system_interface;
    RenderInterface_VK render_interface;
    TextInputMethodEditor_Win32 text_input_method_editor;

    HINSTANCE instance_handle = nullptr;
    std::wstring instance_name;
    HWND window_handle = nullptr;

    bool context_dimensions_dirty = true;
    Rml::Vector2i window_dimensions;
    bool running = true;

    // Arguments set during event processing and nulled otherwise.
    Rml::Context* context = nullptr;
    KeyDownCallback key_down_callback = nullptr;
};

using ProcSetProcessDpiAwarenessContext = BOOL(WINAPI*)(HANDLE value);
using ProcGetDpiForWindow = UINT(WINAPI*)(HWND hwnd);
using ProcAdjustWindowRectExForDpi = BOOL(WINAPI*)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi);
static bool has_dpi_support = false;
static ProcSetProcessDpiAwarenessContext procSetProcessDpiAwarenessContext = NULL;
static ProcGetDpiForWindow procGetDpiForWindow = NULL;
static ProcAdjustWindowRectExForDpi procAdjustWindowRectExForDpi = NULL;

static std::unique_ptr<BackendData> data;

static bool CreateVulkanSurface(VkInstance instance, VkSurfaceKHR* out_surface)
{
    VkWin32SurfaceCreateInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hinstance = GetModuleHandle(NULL);
    info.hwnd = data->window_handle;

    VkResult status = vkCreateWin32SurfaceKHR(instance, &info, nullptr, out_surface);

    bool result = (status == VK_SUCCESS);
    RMLUI_VK_ASSERTMSG(result, "Failed to create Win32 Vulkan surface");
    return result;
}

static void InitializeDpiSupport()
{
    procSetProcessDpiAwarenessContext =
            (ProcSetProcessDpiAwarenessContext)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "SetProcessDpiAwarenessContext");
    procGetDpiForWindow = (ProcGetDpiForWindow)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "GetDpiForWindow");
    procAdjustWindowRectExForDpi =
            (ProcAdjustWindowRectExForDpi)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "AdjustWindowRectExForDpi");
    if (!has_dpi_support && procSetProcessDpiAwarenessContext != NULL && procGetDpiForWindow != NULL && procAdjustWindowRectExForDpi != NULL)
    {
        if (procSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
            has_dpi_support = true;
    }
}

class XnpFrame : public wxFrame
{

public:
    XnpFrame() : wxFrame(nullptr, wxID_ANY, "wx")
    {
        SetIcon(wxICON(IDI_DEALER_ICON));

        // ************************************* Backend::Initialize *************************************
        // data = std::make_unique<BackendData>();
        // data->instance_handle = GetModuleHandle(nullptr);
        // data->instance_name = L"wx";
        // data->window_handle = GetHWND();
        // InitializeDpiSupport();
        // Rml::Vector<const char*> extensions;
        // extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        // extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        // if(!data->render_interface.Initialize(std::move(extensions), CreateVulkanSurface)){
        //     wxLogInfo(L"render_interface Initialize error");
        // }
        // data->system_interface.SetWindow( data->window_handle);
        // data->render_interface.SetViewport(GetClientRect().GetWidth(), GetClientRect().GetHeight());
        // Rml::SetTextInputHandler(&data->text_input_method_editor);
        // // ************************************* Backend::Initialize *************************************
        // Rml::Context* context = Rml::CreateContext("wx", Rml::Vector2i(GetClientRect().GetWidth(), GetClientRect().GetHeight()));
        // if (Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml")){
        //     document->Show();
        // }
    }


protected:
    void OnPaint(wxPaintEvent &event)
    {
        // Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(GetClientRect().GetWidth(), GetClientRect().GetHeight()));
        // if (Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml")){
        //     document->Show();
        // }
        // std::thread windowThread = std::thread([=]() {
        //     bool running = true;
        //     std::thread t;
        //     while (running)
        //     {
        //         context->Update();
        //
        //         // Prepare the backend for taking rendering commands from RmlUi and then render the context.
        //         Backend::BeginFrame();
        //         context->Render();
        //         Backend::PresentFrame();
        //     }
        // });
        // windowThread.detach();

    }
private:
DECLARE_DYNAMIC_CLASS(XnpFrame)

DECLARE_EVENT_TABLE()
};