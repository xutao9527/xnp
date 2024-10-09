#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <wx/graphics.h>
#include <string>
#include <locale>
#include <codecvt>
#include "XnpEventHandler.h"
#include "DbgRenderer.h"


class XnpFrame : public wxFrame
{
    std::weak_ptr<DbgRenderer> rendererContext;
    wxImage image;
public:
    XnpFrame(wxWindow *parent,wxWindowID id,const wxString& title)
    : wxFrame(parent, id, title,wxDefaultPosition,wxSize(512, 384))
    {
        SetIcon(wxICON(IDI_DEALER_ICON));
        // std::shared_ptr<DbgRenderer> context = std::make_shared<DbgRenderer>(GetHWND(),
        //                                                                                  GetTitle().ToStdWstring(),
        //                                                                                  GetClientRect().GetWidth(),
        //                                                                                  GetClientRect().GetHeight());
        // rendererContext = std::weak_ptr<DbgRenderer>(context);
        // context->Run();
        // context.reset();
        // 设置背景颜色为透明
        //SetBackgroundStyle(wxBG_STYLE_PAINT);  // 设置背景样式
        //SetTransparent(150);  // 设置窗体半透明


        // // 创建 wxBitmap 对象
        // wxBitmap bitmap(200, 200);
        // // 创建 wxMemoryDC 对象
        // wxMemoryDC memDC(bitmap);
        // // 使用 wxGraphicsContext 来绘制抗锯齿的图形
        // wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
        // if (gc) {
        //     // 设置没有边框的画笔 (wxTRANSPARENT_PEN)
        //     gc->SetPen(*wxTRANSPARENT_PEN);
        //
        //     // 设置填充颜色为红色
        //     gc->SetBrush(*wxRED_BRUSH);
        //
        //     // 绘制一个带抗锯齿效果的圆角矩形
        //     //gc->DrawRoundedRectangle(0, 0, 200, 200, 50);
        //     int radius = 100; // 圆半径
        //     gc->DrawEllipse(0, 0, radius * 2, radius * 2); // 绘制圆形
        //
        //     // 删除 gc 对象
        //     delete gc;
        // }
        //
        // // 完成绘制后，解除 wxMemoryDC 和 wxBitmap 的关联
        // memDC.SelectObject(wxNullBitmap);
        //
        // wxImage image = bitmap.ConvertToImage();
        // image.InitStandardHandlers();
        // // 保存为 PNG 文件
        // if (image.SaveFile("circle.png")) {
        //     wxLogMessage("Image saved successfully as " );
        // }
        //

        // dc.DrawBitmap(bitmap, 0, 0, false);  // 将 bitmap 绘制到窗口上
    }

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam) override
    {
        if(auto context = rendererContext.lock()){
            // 文本框输出,激活输入法动态候选框位置
            if(message == WM_IME_COMPOSITION){
                context->ActivateKeyboard();
            }
            // 派发事件到自绘引擎
            context->DispatchEvent(message,wParam,lParam);
            // 屏蔽 wxwidgets 本身的事件
            if (message == WM_IME_STARTCOMPOSITION || message == WM_IME_ENDCOMPOSITION || message == WM_IME_COMPOSITION || message == WM_IME_CHAR || message == WM_IME_REQUEST){
                return 0;
            }
        }
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }

    void SetImage(const wxImage& skin){
        image = skin;
    }

    void OnPaint(wxPaintEvent& e);
private:
wxDECLARE_EVENT_TABLE();
};

