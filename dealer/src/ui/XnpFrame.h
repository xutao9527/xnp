#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <wx/graphics.h>
#include <memory>
#include <string>
#include <locale>
#include <codecvt>
#include "XnpEventHandler.h"
#include "DbgRenderer.h"


class XnpFrame : public wxFrame
{
    std::shared_ptr<DbgRenderer> rendererContext;
    wxImage shapeImage;
public:
    XnpFrame(wxWindow *parent,
             wxWindowID id,
             const wxString &title,
             const wxPoint &pos = wxDefaultPosition,
             const wxSize &size = wxDefaultSize,
             long style = wxDEFAULT_FRAME_STYLE,
             const wxString &name = wxASCII_STR(wxFrameNameStr))
            : wxFrame(parent, id, title, pos, size, style, name)
    {
        SetIcon(wxICON(IDI_DEALER_ICON));
        //SetWindowStyle(wxNO_BORDER | wxFRAME_SHAPED);
        rendererContext = std::make_shared<DbgRenderer>(GetHWND(),GetTitle(),GetClientRect().GetWidth(),GetClientRect().GetHeight()
        );
        rendererContext->Run();
        //InitShapeImage();
    }

    ~XnpFrame() override
    {
        std::cout << "~XnpFrame" << std::endl;
    }

    void InitShapeImage()
    {
        wxBitmap bitmap(GetClientRect().GetWidth(), GetClientRect().GetHeight(), 32); // 创建支持透明的位图
        bitmap.UseAlpha(true);
        wxMemoryDC memDC(bitmap);
        // 设置背景为透明
        memDC.SetBackground(wxBrush(wxColour(0, 0, 0, 0))); // 设置透明背景
        memDC.Clear(); // 清空位图以应用透明背景
        // 创建图形上下文
        wxGraphicsContext *gc = wxGraphicsContext::Create(memDC);
        if (gc) {
            gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
            gc->SetBrush(*wxRED_BRUSH); // 设置填充颜色为红色
            gc->SetPen(wxColour(0, 255, 0)); // 设置透明边框
            gc->DrawRoundedRectangle(0, 0, GetClientRect().GetWidth() - 2, GetClientRect().GetHeight() - 2,
                                     8); // 绘制圆角矩形
            delete gc; // 删除图形上下文
        }
        // 完成绘制后，解除 wxMemoryDC 和 wxBitmap 的关联
        memDC.SelectObject(wxNullBitmap);
        shapeImage = bitmap.ConvertToImage();
        if (shapeImage.IsOk()) {
            if (shapeImage.HasAlpha()) {
                shapeImage.ConvertAlphaToMask();
            }
            SetShape(wxRegion(shapeImage.Scale(GetClientRect().GetWidth(), GetClientRect().GetHeight())));
        }
    }

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam) override
    {
        if (auto context = rendererContext) {
            // 文本框输出,激活输入法动态候选框位置
            if (message == WM_IME_COMPOSITION) {
                context->ActivateKeyboard();
            }
            // 派发事件到自绘引擎
            context->DispatchEvent(message, wParam, lParam);
            //屏蔽 wxwidgets IME事件
            if (message == WM_IME_STARTCOMPOSITION || message == WM_IME_ENDCOMPOSITION ||
                message == WM_IME_COMPOSITION || message == WM_IME_CHAR || message == WM_IME_REQUEST) {
                return 0;
            }
        }
        return wxFrame::MSWWindowProc(message, wParam, lParam);
    }
};

