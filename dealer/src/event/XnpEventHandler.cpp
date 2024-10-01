
#include "XnpEventHandler.h"

bool XnpEventHandler::ProcessEvent(wxEvent &event)
{
    if (event.IsKindOf(wxCLASSINFO(wxKeyEvent)))
    {
        wxLogInfo(L"键盘按下事件wx");
    }
    return wxEvtHandler::ProcessEvent(event);
}
