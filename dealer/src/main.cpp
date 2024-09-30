
#include "ui/XnpFrame.h"
#include "shell/Shell.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi_Backend.h>

class XnpApp : public wxApp
{
public:
    bool OnInit() override
    {
        // Shell::Initialize();
        // Rml::Initialise();
        // Shell::LoadFonts();
        // auto * logWindow = new wxLogWindow(nullptr, "Log Window", true, false);
        // logWindow->GetFrame()->SetSize(500,400);
        // logWindow->GetFrame()->SetPosition(wxPoint(10,10));

        other();

        auto *xnpFrame = new XnpFrame();
        xnpFrame->SetSize(500, 320);
        xnpFrame->Center();
        xnpFrame->Show();
        return true;
    }

    int other(){
        const int window_width = 1024;
        const int window_height = 768;

        // Initializes the shell which provides common functionality used by the included samples.
        if (!Shell::Initialize())
            return -1;

        // Constructs the system and render interfaces, creates a window, and attaches the renderer.
        if (!Backend::Initialize("Load Document Sample", window_width, window_height, true))
        {
            Shell::Shutdown();
            return -1;
        }

        // Install the custom interfaces constructed by the backend before initializing RmlUi.
        Rml::SetSystemInterface(Backend::GetSystemInterface());
        Rml::SetRenderInterface(Backend::GetRenderInterface());

        // RmlUi initialisation.
        Rml::Initialise();

        // Create the main RmlUi context.
        Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
        if (!context)
        {
            Rml::Shutdown();
            Backend::Shutdown();
            Shell::Shutdown();
            return -1;
        }

        // The RmlUi debugger is optional but very useful. Try it by pressing 'F8' after starting this sample.
        Rml::Debugger::Initialise(context);

        // Fonts should be loaded before any documents are loaded.
        Shell::LoadFonts();

        // Load and show the demo document.
        if (Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml"))
            document->Show();

        bool running = true;
        while (running)
        {
            // Handle input and window events.
            running = Backend::ProcessEvents(context, &Shell::ProcessKeyDownShortcuts, true);

            // This is a good place to update your game or application.

            // Always update the context before rendering.
            context->Update();

            // Prepare the backend for taking rendering commands from RmlUi and then render the context.
            Backend::BeginFrame();
            context->Render();
            Backend::PresentFrame();
        }

        // Shutdown RmlUi.
        Rml::Shutdown();

        Backend::Shutdown();
        Shell::Shutdown();

        return 0;
    }

    int OnExit() override{
        // Rml::Shutdown();
        // Shell::Shutdown();
        return 0;
    }
};

wxIMPLEMENT_APP(XnpApp);
