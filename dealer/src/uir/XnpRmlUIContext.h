#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <thread>
#include <memory>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi_Backend.h>
#include <Shell.h>


class XnpRmlUIContext: public std::enable_shared_from_this<XnpRmlUIContext>
{
private:
     int window_width = 1024;
     int window_height = 768;

public:
    XnpRmlUIContext(){
        wxLogInfo(L"构造~~~~");
    }

    void Init(){}

    void Run(){
        auto self = shared_from_this();
        std::thread runThread([=]() {
            window_width = 1024;
            window_height = 768;
            Shell::Initialize();
            Backend::Initialize("Load Document Sample", window_width, window_height, true);
            Rml::SetSystemInterface(Backend::GetSystemInterface());
            Rml::SetRenderInterface(Backend::GetRenderInterface());
            Rml::Initialise();
            Shell::LoadFonts();
            self->Loop();
        });
        runThread.detach();
    }

    void Loop(){

        Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
        Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml");
        document->Show();
        bool running = true;
        while (running)
        {
            running = Backend::ProcessEvents(context, &Shell::ProcessKeyDownShortcuts, true);
            context->Update();
            Backend::BeginFrame();
            context->Render();
            Backend::PresentFrame();
        }
    }

    ~XnpRmlUIContext(){
        Rml::Shutdown();
        Backend::Shutdown();
        wxLogInfo(L"析构~~~~");
    }
};


