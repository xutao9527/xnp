
#include "XnpEventHandler.h"

bool XnpEventHandler::ProcessEvent(wxEvent &event)
{
    int eventType = event.GetEventType();

    // 检查事件类型
    //wxLogInfo(wxString::Format(L"捕获到事件，类型ID：%d", eventType));

    if (event.IsKindOf(wxCLASSINFO(wxMouseEvent)))
    {
        wxMouseEvent& mouseEvent = static_cast<wxMouseEvent&>(event);
        if (mouseEvent.Moving()) {
            wxLogInfo(L"捕获到鼠标移动事件");
        } else if (mouseEvent.LeftDown()) {
            wxLogInfo(L"捕获到鼠标左键按下事件");
        } else if (mouseEvent.LeftUp()) {
            wxLogInfo(L"捕获到鼠标左键释放事件");
        } else if (mouseEvent.RightDown()) {
            wxLogInfo(L"捕获到鼠标右键按下事件");
        } else if (mouseEvent.RightUp()) {
            wxLogInfo(L"捕获到鼠标右键释放事件");
        }
    }

    // 调用父类的 ProcessEvent 函数以确保事件继续传递
    return wxEvtHandler::ProcessEvent(event);
}
