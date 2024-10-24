#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#include "Shell.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "RmlUi_Renderer_GL2.h"
#include "RmlUi_Platform_Win32.h"


class XnpWinGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    static std::once_flag initFlag;
    static std::atomic<int> ref_count;  // 静态引用计数器

    SystemInterface_Win32 system_interface;
    RenderInterface_GL2 render_interface;
    TextInputMethodEditor_Win32 text_input_method_editor;
    Rml::Vector2i window_dimensions;
    Rml::Context *context = nullptr;

    HWND window_handle = nullptr;
public:
    explicit XnpWinGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
        // 线程安全，只执行一次
        std::call_once(initFlag, [this]() {
            Rml::Initialise();
            Shell::Initialize();
            Shell::LoadFonts();
            Rml::SetTextInputHandler(&text_input_method_editor);
        });
        ++ref_count;

    }

    ~XnpWinGLWidget() override{
        Rml::ReleaseTextures(&render_interface);
        Rml::RemoveContext("main");
        if (--ref_count == 0) {
            Rml::Shutdown();
            if (Rml::GetTextInputHandler() == &text_input_method_editor)
                Rml::SetTextInputHandler(nullptr);
            Shell::Shutdown();
        }
    }

protected:
    // 初始化 OpenGL 环境
    void initializeGL() override {
        initializeOpenGLFunctions();
        Rml::SetSystemInterface(&system_interface);
        window_handle = reinterpret_cast<HWND>(this->winId());
        system_interface.SetWindow(window_handle);

        render_interface.SetViewport(this->contentsRect().width(), this->contentsRect().height());
        context = Rml::CreateContext("main", Rml::Vector2i(this->contentsRect().width(), this->contentsRect().height()),&render_interface);
        Rml::Debugger::Initialise(context);
        Rml::ElementDocument *document = context->LoadDocument("resources/layout/login.rml");
        document->Show();

        auto *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&QOpenGLWidget::update));
        timer->start(16); // 每16毫秒调用一次，即大约每秒60帧
    }

    // 渲染场景
    void paintGL() override {
        context->Update();
        render_interface.BeginFrame();
        render_interface.Clear();
        context->Render();
        render_interface.EndFrame();
    }

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override {
        MSG *msg = static_cast<MSG *>(message);
        ProcessEvents(msg);
        if (msg->message == WM_IME_STARTCOMPOSITION ||
            msg->message == WM_IME_ENDCOMPOSITION ||
            msg->message == WM_IME_COMPOSITION ||
            msg->message == WM_IME_CHAR ||
            msg->message == WM_IME_REQUEST) {
            return true;
        }
        return QWidget::nativeEvent(eventType, message, result);
    }

    //处理事件
    void ProcessEvents(const MSG *msg){
        switch (msg->message){
            case WM_SIZE: {
                const int width = LOWORD(LOWORD(msg->lParam));
                const int height = HIWORD(msg->lParam);
                window_dimensions.x = width;
                window_dimensions.y = height;
                render_interface.SetViewport(width, height);
                context->SetDimensions(window_dimensions);
                break;
            }
            case WM_KEYDOWN: {
                // Override the default key event callback to add global shortcuts for the samples.
                const Rml::Input::KeyIdentifier rml_key = RmlWin32::ConvertKey((int) msg->wParam);
                const int rml_modifier = RmlWin32::GetKeyModifierState();
                const float native_dp_ratio = float(USER_DEFAULT_SCREEN_DPI) / float(USER_DEFAULT_SCREEN_DPI);
                // See if we have any global shortcuts that take priority over the context.
                if (!ProcessKeyDownShortcuts( rml_key, rml_modifier, native_dp_ratio, true))
                    return ;
                // Otherwise, hand the event over to the context by calling the input handler as normal.
                if (!RmlWin32::WindowProcedure(context, text_input_method_editor, window_handle, msg->message, msg->wParam,msg->lParam))
                    return ;
                // The key was not consumed by the context either, try keyboard shortcuts of lower priority.
                if (!ProcessKeyDownShortcuts( rml_key, rml_modifier, native_dp_ratio, false))
                    return ;
            }
            default:{
                RmlWin32::WindowProcedure(context, text_input_method_editor, window_handle, msg->message, msg->wParam,msg->lParam);
            }
        }
    }

    bool ProcessKeyDownShortcuts(Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority){
        bool result = false;
        if (priority)
        {
            if (key == Rml::Input::KI_F8)
            {
                Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
            }
            else if (key == Rml::Input::KI_0 && key_modifier & Rml::Input::KM_CTRL)
            {
                context->SetDensityIndependentPixelRatio(native_dp_ratio);
            }
            else if (key == Rml::Input::KI_1 && key_modifier & Rml::Input::KM_CTRL)
            {
                context->SetDensityIndependentPixelRatio(1.f);
            }
            else if ((key == Rml::Input::KI_OEM_MINUS || key == Rml::Input::KI_SUBTRACT) && key_modifier & Rml::Input::KM_CTRL)
            {
                const float new_dp_ratio = Rml::Math::Max(context->GetDensityIndependentPixelRatio() / 1.2f, 0.5f);
                context->SetDensityIndependentPixelRatio(new_dp_ratio);
            }
            else if ((key == Rml::Input::KI_OEM_PLUS || key == Rml::Input::KI_ADD) && key_modifier & Rml::Input::KM_CTRL)
            {
                const float new_dp_ratio = Rml::Math::Min(context->GetDensityIndependentPixelRatio() * 1.2f, 2.5f);
                context->SetDensityIndependentPixelRatio(new_dp_ratio);
            }
            else
            {
                result = true;
            }
        }
        else
        {
            if (key == Rml::Input::KI_R && key_modifier & Rml::Input::KM_CTRL)
            {
                for (int i = 0; i < context->GetNumDocuments(); i++)
                {
                    Rml::ElementDocument* document = context->GetDocument(i);
                    const Rml::String& src = document->GetSourceURL();
                    if (src.size() > 4 && src.substr(src.size() - 4) == ".rml")
                    {
                        document->ReloadStyleSheet();
                    }
                }
            }
            else
            {
                result = true;
            }
        }
        return result;
    }
};

