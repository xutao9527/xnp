#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

struct Win32VkEvent
{
    UINT message;
    WPARAM w_param;
    LPARAM l_param;

    Win32VkEvent(UINT msg, WPARAM wp, LPARAM lp)
            : message(msg), w_param(wp), l_param(lp) {}
};

class XnpWin32VKContext : public std::enable_shared_from_this<XnpWin32VKContext>
{
private:
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<Win32VkEvent> eventQueue;
    std::atomic<bool> running;
public:
    XnpWin32VKContext()
    {
        int a = 0;
        wxLogMessage(L"构造%d", a);
    }

    void PushEvent(UINT message, WPARAM w_param, LPARAM l_param)
    {
        wxLogMessage(L"添加事件:%d", message);
        eventQueue.emplace(message, w_param, l_param);
        cv.notify_all();
    }

    void Run()
    {
        running = true;
        auto self = shared_from_this();
        std::thread loopThread([=]{
            self->Loop();
        });
        loopThread.detach();
    }

    void Loop()
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (running) {
            cv.wait(lock, [=] {
                bool rvl = false;
                while (!eventQueue.empty()){
                    Win32VkEvent event = eventQueue.front();
                    eventQueue.pop();
                    wxLogMessage(L"处理事件:%d", event.message);
                    rvl = true;
                }
                return rvl;
            });
        }
    }

    ~XnpWin32VKContext()
    {
        int a = 0;
        wxLogMessage(L"析构%d", a);
    }
};



