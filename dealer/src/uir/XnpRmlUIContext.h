#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <RmlUi_Backend.h>
#include <thread>
#include <memory>
#include <utility>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <Shell.h>
#include <atomic>
#include <mutex>
#include <condition_variable>

struct MessageData {
    UINT message;
    WPARAM w_param;
    LPARAM l_param;

    MessageData(UINT msg, WPARAM wp, LPARAM lp)
            : message(msg), w_param(wp), l_param(lp) {}
};

class XnpRmlUIContext: public std::enable_shared_from_this<XnpRmlUIContext>
{
private:
    std::string window_title;
    int window_width;
    int window_height;
    HWND window_handle;
    std::atomic<bool> m_running;

    std::queue<MessageData> message_queue;
    std::mutex mutex;
    std::condition_variable cv;
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

    LRESULT EventHandler(UINT message, WPARAM w_param, LPARAM l_param){
        if(m_running){
            //if(message!=70&&message!=36&&message!=131&&message!=71){
            //wxLogInfo("EventHandler: %d", message);
            //return Backend::WindowProcedureHandler(window_handle, message, w_param, l_param);
            //}
            message_queue.emplace(message, w_param, l_param);
            cv.notify_all();
        }
        return 0;
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
        Rml::Debugger::Initialise(context);
        Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml");
        document->Show();
        bool running = true;
        m_running = true;
        Backend::SetContext(context,&Shell::ProcessKeyDownShortcuts);
        using clock = std::chrono::high_resolution_clock;
        auto last_time = clock::now();
        int frame_count = 0;
        double fps = 0.0;
        const int fps_update_interval = 10; // 每100帧更新一次FPS
        while (running)
        {
            //running = Backend::ProcessEvents(context, &Shell::ProcessKeyDownShortcuts, true);
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [=] {
                bool result = false;
                while (!message_queue.empty()){
                    MessageData msg_data = message_queue.front();
                    message_queue.pop();  // 移除消息
                    Backend::ProcessEvents(context,&Shell::ProcessKeyDownShortcuts,true,window_handle, msg_data.message, msg_data.w_param, msg_data.l_param);
                    result = true;
                }
                return result;
            });

            context->Update();
            Backend::BeginFrame();
            context->Render();
            Backend::PresentFrame();
            // 计算帧率
            auto current_time = clock::now();
            frame_count++;

            //wxLogMessage("Frame count: %d", frame_count);
            if (frame_count >= fps_update_interval) {
                std::chrono::duration<double> elapsed = current_time - last_time;
                fps = frame_count / elapsed.count();
                wxLogMessage("FPS: %.2f", fps);  // 使用 wxWidgets 进行日志输出

                // 重置计数器
                frame_count = 0;
                last_time = current_time;
            }
        }
    }

    ~XnpRmlUIContext(){
        Rml::Shutdown();
        Backend::Shutdown();
        wxLogInfo(L"析构~~~~");
    }
};


