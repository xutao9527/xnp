#include "wxSkinXFrame.h"
#include <wx/dcbuffer.h>
IMPLEMENT_DYNAMIC_CLASS(wxSkinXFrame,wxFrame)

BEGIN_EVENT_TABLE(wxSkinXFrame,wxFrame)
                //关联擦除背景函数
                EVT_ERASE_BACKGROUND(wxSkinXFrame::OnErase)
                //关联绘制函数
                EVT_PAINT(wxSkinXFrame::OnPaint)
                //关联窗口大小发生改变函数
                EVT_SIZE(wxSkinXFrame::OnSize)
                //关联鼠标进入窗口函数
                EVT_ENTER_WINDOW(wxSkinXFrame::OnEnterWindow)
                //关联鼠标离开窗口函数
                EVT_LEAVE_WINDOW(wxSkinXFrame::OnLeaveWindow)
                //鼠标移动事件
                EVT_LEFT_DOWN(wxSkinXFrame::OnLeftDown)
                EVT_LEFT_UP(wxSkinXFrame::OnLeftUp)
                EVT_MOUSE_CAPTURE_LOST(wxSkinXFrame::OnMouseLost)
                EVT_MOTION(wxSkinXFrame::OnMotion)
END_EVENT_TABLE()
wxSkinXFrame::wxSkinXFrame()
{
    Init();
}
wxSkinXFrame::wxSkinXFrame(wxWindow* parent,
                           wxWindowID id,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style,
                           const wxString& name)
{
    Init();
    Create(parent,id,pos,size,style,name);
}
void wxSkinXFrame::Create(wxWindow *parent,
                          wxWindowID id,
                          const wxPoint& pos,
                          const wxSize& size,
                          long style,
                          const wxString& name)
{
    wxFrame::Create(parent,id,"",pos,size,style,name);
}
void wxSkinXFrame::Init()
{
    m_bInside = false;

    m_rect = wxRect(0,0,0,0);
    m_offset = wxPoint(-1,-1);
}
wxSkinXFrame::~wxSkinXFrame()
{

}


bool wxSkinXFrame::SetShape(const wxImage& img)
{
    wxImage m = img;
    if(m.HasAlpha())
    {
        m.ConvertAlphaToMask();
    }
    return wxNonOwnedWindow::SetShape(wxRegion(m));
}
void wxSkinXFrame::OnPaint(wxPaintEvent& e)
{
    //如果窗体长宽有一个为0，则不进行绘制
    int w, h;
    GetSize(&w,&h);
    if(w == 0 || h == 0)
    {
        return;
    }
    //使用双缓冲绘图，防止闪烁
    wxBufferedPaintDC dcp(this);
    //清除背景
    dcp.SetBrush(*wxTRANSPARENT_BRUSH);
    dcp.Clear();
    //在窗口上绘制图片，默认都是按照窗体大小进行等比例绘制
    if(m_bInside && image_over.IsOk())
    {
        //当鼠标在窗口里面的时候
        dcp.DrawBitmap(wxBitmap(image_over.Scale(w, h)),0,0,true);
    }
    else if(IsEnabled() && image_normal.IsOk())
    {
        //当窗口不属于禁用状态并且有正常的皮肤图片时
        dcp.DrawBitmap(wxBitmap(image_normal.Scale(w, h)),0,0,true);
    }
    else if(!IsEnabled() && image_disable.IsOk() )
    {
        //当窗口处于禁用状态时
        dcp.DrawBitmap(wxBitmap(image_disable.Scale(w, h)),0,0,true);
    }else if(image_normal.IsOk())
    {
        //当窗口不属于上面几种条件时，默认绘制正常的皮肤图片
        dcp.DrawBitmap(wxBitmap(image_normal.Scale(w, h)),0,0,true);
    }

}
void wxSkinXFrame::OnSize(wxSizeEvent& e)
{
    ReSetShape();
}
void wxSkinXFrame::OnEnterWindow(wxMouseEvent& event)
{
    m_bInside = true;
    if(image_over.IsOk())
    {
        Refresh();
    }
}
void wxSkinXFrame::OnLeaveWindow(wxMouseEvent& event)
{
    m_bInside = false;
    Refresh();
    //鼠标拖动事件，当鼠标离开窗口时，发送信号告诉窗口
    //这个是为了解决拖动速度过快导致窗口没有跟踪上鼠标
    bool isDown = event.LeftIsDown();
    bool isDragging = event.Dragging();
    if(isDown && isDragging)
    {
        wxCommandEvent ev(wxEVT_MOTION, GetId());
        ev.SetEventObject(this);
        GetEventHandler()->ProcessEvent(ev);
    }
}
//鼠标移动的时候
void wxSkinXFrame::OnMotion(wxMouseEvent& event)
{
    wxPoint pt = event.GetPosition();
    bool isDown = event.LeftIsDown();

    if (isDown && event.Dragging() && HasCapture())
    {

        wxPoint mouse_pos = ClientToScreen(pt);

        if(m_offset.x != -1)
        {
            wxPoint dp = mouse_pos - m_offset;
            this->Move(dp);
        }

    }
}
//鼠标左键按下的时候
void wxSkinXFrame::OnLeftDown(wxMouseEvent& event)
{
    // 当前鼠标的屏幕坐标
    wxPoint pt = event.GetPosition();

    if(!HasCapture() && !m_rect.Contains(pt))
    {
        CaptureMouse();
    }

    wxPoint mouse_pos = ClientToScreen(pt);


    // 当前主窗口的屏幕坐标
    wxPoint wnd_pos = this->GetPosition();

    // 计算鼠标的坐标点到窗口左上角的偏移量
    m_offset.x = mouse_pos.x - wnd_pos.x;
    m_offset.y = mouse_pos.y - wnd_pos.y;


}
//鼠标左键松开的时候
void wxSkinXFrame::OnLeftUp(wxMouseEvent& event)
{
    m_offset = wxPoint(-1,-1);
    if (HasCapture()) ReleaseMouse();
}
//鼠标丢失的时候
void wxSkinXFrame::OnMouseLost(wxMouseCaptureLostEvent& event)
{
    m_offset = wxPoint(-1,-1);
    if (HasCapture()) ReleaseMouse();
}



void wxSkinXFrame::ReSetShape()
{
    if(image_shape.IsOk())
    {
        //当窗口大小改变是可以改变窗口的形状
        int w, h;
        GetSize(&w,&h);
        if(w != 0 && h != 0)
        {
            SetShape(image_shape.Scale(w,h));
        }
    }

    Layout();
    Refresh();
}