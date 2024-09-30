
// #include "XnpFrame.h"
// #include "XnpRWindow.h"
// #include "XnpRmlUIContext.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "XnpRmlUIContext.h"
#include "XnpFrame.h"

class XnpApp : public wxApp
{

public:
    bool OnInit() override
    {
        auto * logWindow = new wxLogWindow(nullptr, "Log Window", true, false);
        logWindow->GetFrame()->SetSize(500,400);
        logWindow->GetFrame()->SetPosition(wxPoint(10,10));

        auto *xnpFrame = new XnpFrame();
        xnpFrame->SetSize(500, 320);
        xnpFrame->Center();
        xnpFrame->Show();
        return true;
    }



    int OnExit() override{
        // Rml::Shutdown();
        // Shell::Shutdown();
        return 0;
    }
};

wxIMPLEMENT_APP(XnpApp);
