

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "XnpFrame.h"

class XnpApp : public wxApp
{
public:
    bool OnInit() override
    {
        wxInitAllImageHandlers();

        auto * logWindow = new wxLogWindow(nullptr, "Log Window", true, false);
        logWindow->GetFrame()->SetSize(700,400);
        logWindow->GetFrame()->SetPosition(wxPoint(10,10));

        auto *xnpFrame = new XnpFrame(nullptr,wxID_ANY,L"绘图引擎",wxPoint(0,0),wxSize(512,354));
        xnpFrame->Center();
        xnpFrame->Show();
        return true;
    }

    int OnExit() override
    {
        std::cout << "OnExit" << std::endl;
        return 0;
    }
};

wxIMPLEMENT_APP(XnpApp);
