#include <wx/wx.h>
#include "ui/XnpFrame.h"

class MyApp : public wxApp {
public:
     bool OnInit() override {
        wxInitAllImageHandlers();
        auto* f = new wxSkinXFrame(nullptr,wxID_ANY);
        f->SetNormalImage(wxImage("resources/12041.jpg"));
        f->SetShapeImage(wxImage("resources/demo.png"));
        f->SetOverImage(wxImage("resources/12043.jpg"));
        f->SetDisableImage(wxImage("resources/12044.jpg"));
        f->SetSize(500,320);
        f->Center();
        f->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
