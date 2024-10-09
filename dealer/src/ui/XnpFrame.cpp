#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include "XnpFrame.h"

// IMPLEMENT_DYNAMIC_CLASS(XnpFrame, wxFrame)

wxBEGIN_EVENT_TABLE(XnpFrame, wxFrame)
                EVT_PAINT(XnpFrame::OnPaint)
wxEND_EVENT_TABLE()

void XnpFrame::OnPaint(wxPaintEvent& e){

    // wxBitmap bitmap(GetClientRect().GetWidth(), GetClientRect().GetHeight()); // 创建支持透明的位图
    // bitmap.UseAlpha(true);
    // wxMemoryDC memDC(bitmap);
    // // 设置背景为透明
    // memDC.SetBackground(wxBrush(wxColour(0, 0, 0, 0))); // 设置透明背景
    // memDC.Clear(); // 清空位图以应用透明背景
    // // 创建图形上下文
    // wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
    // if (gc) {
    //     gc->SetBrush(*wxRED_BRUSH); // 设置填充颜色为红色
    //     gc->SetPen(*wxTRANSPARENT_PEN); // 设置透明边框
    //     gc->DrawRoundedRectangle(0, 0, GetClientRect().GetWidth(), GetClientRect().GetHeight(), 10); // 绘制圆角矩形
    //     delete gc; // 删除图形上下文
    // }
    // // 完成绘制后，解除 wxMemoryDC 和 wxBitmap 的关联
    // memDC.SelectObject(wxNullBitmap);
    //
    // //使用双缓冲绘图，防止闪烁
    // wxBufferedPaintDC dcp(this);
    // // 在窗口中绘制 wxBitmap
    // dcp.DrawBitmap(bitmap, 0, 0, true);

}