#include <wx/wx.h>
#include "ui/XnpFrame.h"

class MyApp : public wxApp {
public:
     bool OnInit() override {
        wxInitAllImageHandlers();
        XnpFrame* xnpFrame = new XnpFrame();
        xnpFrame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
