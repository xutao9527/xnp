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
    wxTextCtrl* textCtrl;  // 文本输入框
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

        // 创建一个文本输入框
        textCtrl = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition);

        // 设置布局
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(textCtrl, 0, wxALL | wxEXPAND, 10);
        SetSizer(sizer);
    }

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam) override
    {
        if(auto context = rendererContext.lock()){
            context->DispatchEvent(message,wParam,lParam);
        }
        if (message == WM_IME_STARTCOMPOSITION ||
            message == WM_IME_ENDCOMPOSITION ||
            message == WM_IME_COMPOSITION ||
            message == WM_IME_CHAR ||
            message == WM_IME_REQUEST)
        {
            return 0; // 阻止 IME 相关消息的处理
        }
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }
private:
wxDECLARE_EVENT_TABLE();
};

