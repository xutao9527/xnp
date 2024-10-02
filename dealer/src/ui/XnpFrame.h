#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <string>
#include <locale>
#include <codecvt>
#include "XnpEventHandler.h"
#include "XnpRmlUIContext.h"
#include "XnpWin32VKContext.h"


class XnpFrame : public wxFrame
{
    //std::weak_ptr<XnpRmlUIContext> xnpRmlUIContext;
    //HWND window_handle;

    std::weak_ptr<XnpWin32VKContext> xnpWin32VKContext;
public:
    XnpFrame(wxWindow *parent,wxWindowID id,const wxString& title) : wxFrame(parent, id, title)
    {
        SetIcon(wxICON(IDI_DEALER_ICON));
        std::string sTitle = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(GetTitle().ToStdWstring()) ;
        std::shared_ptr<XnpWin32VKContext> context = std::make_shared<XnpWin32VKContext>(GetHWND(),
                                                                                         sTitle,
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
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }


private:
wxDECLARE_EVENT_TABLE();
};

