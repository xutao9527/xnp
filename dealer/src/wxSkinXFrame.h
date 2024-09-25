#ifndef WXSKINXFRAME_H
#define WXSKINXFRAME_H
#include <wx/wx.h>
#include <wx/image.h>

class wxSkinXFrame : public wxFrame
{
    //构造、析构、初始化
public:
    wxSkinXFrame();
    wxSkinXFrame(wxWindow* parent,
                 wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxNO_BORDER|wxFRAME_SHAPED, //自绘一般默认为无边框
                 const wxString& name = wxFrameNameStr);
    void Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxNO_BORDER | wxFRAME_SHAPED,//自绘一般默认为无边框
                const wxString& name = wxFrameNameStr) ;
    void Init();
    ~wxSkinXFrame();
public:
    //设置皮肤
    void SetShapeImage(const wxImage& skin){image_shape = skin;ReSetShape();}
    void SetNormalImage(const wxImage& skin){image_normal = skin;Refresh();}
    void SetOverImage(const wxImage& skin){image_over = skin;Refresh();};
    void SetDisableImage(const wxImage& skin){image_disable = skin;Refresh();};
public:
    //设置窗口形状
    bool SetShape(const wxImage& img);
protected:
    void OnErase(wxEraseEvent& e){};//擦除函数
    void OnPaint(wxPaintEvent& e);//绘制函数
    void OnSize(wxSizeEvent& e);//当窗口大小发生改变的时候
    void OnEnterWindow(wxMouseEvent& event);//鼠标进入窗口的时候
    void OnLeaveWindow(wxMouseEvent& event);//鼠标离开窗口的时候
protected:
    void ReSetShape();
protected:
    //关于移动窗口的函数
    void OnMotion(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnMouseLost(wxMouseCaptureLostEvent& event);
private:
    wxImage image_shape; //窗口形状
    wxImage image_normal; //窗口皮肤
    wxImage image_over;//鼠标在窗口上面的皮肤
    wxImage image_disable;//窗口被禁用后的皮肤

    bool m_bInside;//鼠标是否在窗口里面

    wxPoint m_offset;//偏移坐标
    wxRect m_rect;
private:
    //将类加入到RTTI中
DECLARE_DYNAMIC_CLASS(wxSkinXFrame)
    //定义事件表
DECLARE_EVENT_TABLE()
};

#endif // WXSKINXFRAME_H