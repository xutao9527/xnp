#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include "event/XnpEventHandler.h"
#include "XnpRmlUIContext.h"

class XnpFrame : public wxFrame
{

public:
    XnpFrame() : wxFrame(nullptr, wxID_ANY, "wx")
    {
        SetIcon(wxICON(IDI_DEALER_ICON));

        // std::shared_ptr<XnpRmlUIContext> x = std::make_shared<XnpRmlUIContext>();
        // x->Init("wx", 512, 384, GetHWND());
        // x->Run();
        XnpEventHandler* handler = new XnpEventHandler();
        this->PushEventHandler(handler);
    }


protected:
    void OnPaint(wxPaintEvent &event)
    {
        wxLogInfo(L"绘制!");
    }

private:
wxDECLARE_EVENT_TABLE();
};

