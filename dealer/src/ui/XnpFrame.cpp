#include "XnpFrame.h"
#include <wx/dcbuffer.h>

IMPLEMENT_DYNAMIC_CLASS(XnpFrame, wxFrame)

BEGIN_EVENT_TABLE(XnpFrame, wxFrame)
                EVT_PAINT(XnpFrame::OnPaint)
END_EVENT_TABLE()

void XnpFrame::OnPaint(wxPaintEvent &event)
{
    wxLogInfo(L"绘制窗口");
    wxWindow::OnPaint(event);
}

