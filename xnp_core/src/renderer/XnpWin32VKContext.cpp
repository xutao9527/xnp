#include "XnpWin32VKContext.h"

std::once_flag XnpWin32VKContext::initFlag;
std::atomic<int> XnpWin32VKContext::ref_count(0);
std::atomic<HWND> XnpWin32VKContext::hwnd{ nullptr };

void XnpWin32VKContext::DispatchEvent(UINT message, WPARAM w_param, LPARAM l_param)
{
    //wxLogMessage(L"添加事件:%d", message);
    eventQueue.emplace(message, w_param, l_param);
    cv.notify_all();
}

bool XnpWin32VKContext::ProcessEvents(UINT message, WPARAM w_param, LPARAM l_param)
{
    if (context_dimensions_dirty) {
        context_dimensions_dirty = false;
        const float dp_ratio = GetDensityIndependentPixelRatio();
        context->SetDimensions(window_dimensions);
        context->SetDensityIndependentPixelRatio(dp_ratio);
    }
    WindowProcedureHandler(message, w_param, l_param);
    while (!render_interface.IsSwapchainValid()) {
        if (!render_interface.IsSwapchainValid())
            render_interface.RecreateSwapchain();
    }
    return running;
}

LRESULT CALLBACK XnpWin32VKContext::WindowProcedureHandler(UINT message, WPARAM w_param, LPARAM l_param)
{
    switch (message) {
        case WM_CLOSE: {
            running = false;
            return 0;
        }
        case WM_SIZE: {
            const int width = LOWORD(l_param);
            const int height = HIWORD(l_param);
            window_dimensions.x = width;
            window_dimensions.y = height;
            if (context) {
                render_interface.SetViewport(width, height);
                context->SetDimensions(window_dimensions);
            }
            return 0;
        }
        case WM_DPICHANGED: {
            RECT *new_pos = (RECT *) l_param;
            SetWindowPos(window_handle, nullptr, new_pos->left, new_pos->top, new_pos->right - new_pos->left,
                         new_pos->bottom - new_pos->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);
            if (context && has_dpi_support)
                context->SetDensityIndependentPixelRatio(GetDensityIndependentPixelRatio());
            return 0;
        }
        case WM_KEYDOWN: {
            // Override the default key event callback to add global shortcuts for the samples.
            const Rml::Input::KeyIdentifier rml_key = RmlWin32::ConvertKey((int) w_param);
            const int rml_modifier = RmlWin32::GetKeyModifierState();
            const float native_dp_ratio = GetDensityIndependentPixelRatio();
            // See if we have any global shortcuts that take priority over the context.
            if (!ProcessKeyDownShortcuts(context, rml_key, rml_modifier, native_dp_ratio, true))
                return 0;
            // Otherwise, hand the event over to the context by calling the input handler as normal.
            if (!RmlWin32::WindowProcedure(context, text_input_method_editor, window_handle, message, w_param,l_param))
                return 0;
            // The key was not consumed by the context either, try keyboard shortcuts of lower priority.
            if (!ProcessKeyDownShortcuts(context, rml_key, rml_modifier, native_dp_ratio, false))
                return 0;
            return 0;
        }
        case WM_LBUTTONDOWN:{
            if (!RmlWin32::WindowProcedure(context, text_input_method_editor, window_handle, message, w_param,l_param))
                return 0;
        }
        default: {
            // Submit it to the platform handler for default input handling.
            if (!RmlWin32::WindowProcedure(context, text_input_method_editor, window_handle, message, w_param,l_param))
                return 0;
        }
    }
    return 0;
    //return DefWindowProc(window_handle, message, w_param, l_param);
}

float XnpWin32VKContext::GetDensityIndependentPixelRatio()
{
    auto GetWindowDpi = [this]() -> UINT {
        if (has_dpi_support) {
            UINT dpi = procGetDpiForWindow(window_handle);
            if (dpi != 0)
                return dpi;
        }
        return USER_DEFAULT_SCREEN_DPI;
    };
    return float(GetWindowDpi()) / float(USER_DEFAULT_SCREEN_DPI);
}

void XnpWin32VKContext::InitializeDpiSupport()
{
    procSetProcessDpiAwarenessContext =
            (ProcSetProcessDpiAwarenessContext)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "SetProcessDpiAwarenessContext");
    procGetDpiForWindow = (ProcGetDpiForWindow)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "GetDpiForWindow");
    procAdjustWindowRectExForDpi =
            (ProcAdjustWindowRectExForDpi)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "AdjustWindowRectExForDpi");

    if (!has_dpi_support && procSetProcessDpiAwarenessContext != NULL && procGetDpiForWindow != NULL && procAdjustWindowRectExForDpi != NULL)
    {
        // Activate Per Monitor V2.
        if (procSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
            has_dpi_support = true;
    }
}