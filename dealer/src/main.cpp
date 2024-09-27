
#include "ui/XnpFrame.h"

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
};

wxIMPLEMENT_APP(XnpApp);
