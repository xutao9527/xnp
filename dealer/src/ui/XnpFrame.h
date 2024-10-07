#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <string>
#include <locale>
#include <codecvt>
#include "XnpEventHandler.h"
#include "DbgRenderer.h"


class XnpFrame : public wxFrame
{
    std::weak_ptr<DbgRenderer> rendererContext;
public:
    XnpFrame(wxWindow *parent,wxWindowID id,const wxString& title) : wxFrame(parent, id, title)
    {
        SetIcon(wxICON(IDI_DEALER_ICON));
        std::shared_ptr<DbgRenderer> context = std::make_shared<DbgRenderer>(GetHWND(),
                                                                                         GetTitle().ToStdWstring(),
                                                                                         GetClientRect().GetWidth(),
                                                                                         GetClientRect().GetHeight());
        rendererContext = std::weak_ptr<DbgRenderer>(context);
        context->Run();
        context.reset();
    }

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam) override
    {
        if(auto context = rendererContext.lock()){
            context->DispatchEvent(message,wParam,lParam);
        }
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }
private:
wxDECLARE_EVENT_TABLE();
};

