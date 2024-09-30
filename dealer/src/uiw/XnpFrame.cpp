#include "XnpFrame.h"
IMPLEMENT_DYNAMIC_CLASS(XnpFrame, wxFrame)

BEGIN_EVENT_TABLE(XnpFrame, wxFrame)
                EVT_PAINT(XnpFrame::OnPaint)
END_EVENT_TABLE()
