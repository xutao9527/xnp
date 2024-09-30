#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <thread>
#include <memory>
#include <utility>
#include <RmlUi_Backend.h>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <Shell.h>


class XnpRmlUIContext: public std::enable_shared_from_this<XnpRmlUIContext>
{
private:
    std::string window_title;
    int window_width;
    int window_height;
    HWND window_handle;

public:
    XnpRmlUIContext(){
        wxLogInfo(L"构造~~~~");
    }

    void Init(std::string title,int width,int height,HWND hwnd){
        window_title = std::move(title);
        window_width = width;
        window_height = height;
        window_handle = hwnd;
    }

    void Run(){
        auto self = shared_from_this();
        std::thread runThread([=]() {
            Shell::Initialize();
            Backend::Initialize(window_title.c_str(), window_width, window_height, true ,window_handle);
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
            //wxLogInfo(L"绘制!");
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


