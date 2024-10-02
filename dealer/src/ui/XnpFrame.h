#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include "XnpEventHandler.h"
#include "XnpRmlUIContext.h"
#include "XnpWin32VKContext.h"

class XnpFrame : public wxFrame
{
    //std::weak_ptr<XnpRmlUIContext> xnpRmlUIContext;
    //HWND window_handle;

    std::weak_ptr<XnpWin32VKContext> xnpWin32VKContext;
public:
    XnpFrame() : wxFrame(nullptr, wxID_ANY, "wx")
    {
        SetIcon(wxICON(IDI_DEALER_ICON));
        // window_handle = GetHWND();
        // {
        //     std::shared_ptr<XnpRmlUIContext> context = std::make_shared<XnpRmlUIContext>();
        //     context->Init("wx", 512, 384, window_handle);
        //     context->Run();
        //     xnpRmlUIContext = std::weak_ptr<XnpRmlUIContext>(context);
        // }

        std::shared_ptr<XnpWin32VKContext> context = std::make_shared<XnpWin32VKContext>(GetHWND(),
                                                                                         GetTitle().ToStdString(),
                                                                                         GetClientRect().GetWidth(),
                                                                                         GetClientRect().GetHeight());
        xnpWin32VKContext = std::weak_ptr<XnpWin32VKContext>(context);
        context->Run();
        context.reset();
    }

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam) override
    {
        if(auto context = xnpWin32VKContext.lock()){
            context->PushEvent(message,wParam,lParam);
        }
        // if(auto context = xnpRmlUIContext.lock()){
        //    context->EventHandler(message,wParam,lParam);
        // }
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }


private:
wxDECLARE_EVENT_TABLE();
};

