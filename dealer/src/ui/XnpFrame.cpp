#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include "XnpFrame.h"

// IMPLEMENT_DYNAMIC_CLASS(XnpFrame, wxFrame)

wxBEGIN_EVENT_TABLE(XnpFrame, wxFrame)
                // EVT_PAINT(XnpFrame::OnPaint)
wxEND_EVENT_TABLE()

void XnpFrame::OnPaint(wxPaintEvent& e){

    // wxPaintDC dc(this);
    // wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    //
    // if (gc) {
    //     // 启用抗锯齿
    //     gc->SetAntialiasMode(wxANTIALIAS_NONE);
    //     gc->SetBrush(*wxRED_BRUSH);
    //     gc->DrawRoundedRectangle(0, 0, 400, 300, 50); // 绘制圆角矩形
    //
    //     delete gc;
    // }

}