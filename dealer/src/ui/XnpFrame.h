#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class XnpFrame : public wxFrame
{
public:
    XnpFrame() : wxFrame(nullptr, wxID_ANY, "xw")
    {
        SetIcon(wxICON(IDI_DEALER_ICON));
    }
protected:
    void OnPaint(wxPaintEvent& event);
private:
    //将类加入到RTTI中
DECLARE_DYNAMIC_CLASS(XnpFrame)
    //定义事件表
DECLARE_EVENT_TABLE()
};


