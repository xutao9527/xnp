
#include "XnpEventHandler.h"

bool XnpEventHandler::ProcessEvent(wxEvent &event)
{
    //wxLogDebug("捕获到事件: %d", event.GetEventType());
    return wxEvtHandler::ProcessEvent(event);
}
