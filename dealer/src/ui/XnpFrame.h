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
    void OnPaint(wxPaintEvent &event)
    {
        // 获取当前窗口设备上下文
        wxPaintDC dc(this);
        HDC hdc = dc.GetHDC();
        dc.DrawText("Hello, world!", 20, 20);

    }



private:
DECLARE_DYNAMIC_CLASS(XnpFrame)

DECLARE_EVENT_TABLE()
};