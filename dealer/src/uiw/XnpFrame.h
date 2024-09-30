#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include "event/XnpEventHandler.h"
#include "XnpRmlUIContext.h"

class XnpFrame : public wxFrame
{
    std::weak_ptr<XnpRmlUIContext> xnpRmlUIContext;
    HWND window_handle;
public:
    XnpFrame() : wxFrame(nullptr, wxID_ANY, "wx")
    {
        SetIcon(wxICON(IDI_DEALER_ICON));
        window_handle = GetHWND();
        {
            std::shared_ptr<XnpRmlUIContext> context = std::make_shared<XnpRmlUIContext>();
            context->Init("wx", 512, 384, window_handle);
            context->Run();
            xnpRmlUIContext = std::weak_ptr<XnpRmlUIContext>(context);
        }
    }

    ~XnpFrame() override {

    }


    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam) override
    {
        if(auto context = xnpRmlUIContext.lock()){
            context->EventHandler(message,wParam,lParam);
        }
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }

protected:
    void OnPaint(wxPaintEvent &event)
    {
        // wxLogInfo(L"绘制!");
    }

private:
wxDECLARE_EVENT_TABLE();
};

