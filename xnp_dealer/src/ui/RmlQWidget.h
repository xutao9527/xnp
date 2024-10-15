#pragma once

#include <QWidget>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <windows.h>
#include <QLineEdit>
#include "DbgRenderer.h"

class RmlQWidget : public QWidget
{
    std::shared_ptr<DbgRenderer> rendererContext;
    std::shared_ptr<QThread> thread; // 使用智能指针管理 QThread
public:
    explicit RmlQWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    ~RmlQWidget() override = default;


public slots:
    void ActivateImm() {
        HIMC himc = ImmGetContext(reinterpret_cast<HWND>(this->winId()));
        if (himc == nullptr) {
            HIMC newHimc = ImmCreateContext();
            ImmAssociateContext(reinterpret_cast<HWND>(this->winId()), newHimc);
        }
    }
protected:
    void showEvent(QShowEvent *event) override
    {
        if(!rendererContext.get()){
            rendererContext = std::make_shared<DbgRenderer>( reinterpret_cast<HWND>(this->winId()), this->windowTitle().toStdString(), this->contentsRect().width(), this->contentsRect().height());
            thread = std::make_shared<QThread>(); // 创建线程
            rendererContext->moveToThread(thread.get());
            connect(thread.get(), &QThread::started, rendererContext.get(), &DbgRenderer::Run);
            connect(rendererContext.get(), &DbgRenderer::ActivateImm, this, &RmlQWidget::ActivateImm);
            thread->start();
        }
        QWidget::showEvent(event);
    }

    void closeEvent(QCloseEvent *event) override
    {
        if (auto t = rendererContext->thread()) {
            t->quit();
            t->wait();
        }
        rendererContext.reset();
        thread.reset();
        QWidget::closeEvent(event);
    }

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override
    {
        MSG *msg = static_cast<MSG *>(message);
        if (auto context = rendererContext) {
            // 文本框输出,激活输入法动态候选框位置
            if (msg->message == WM_IME_COMPOSITION) {
                context->ActivateKeyboard();    // 激活输入法动态候选框位置
            }
            //屏蔽IME事件,直接传递给自绘引擎
            if (msg->message == WM_IME_STARTCOMPOSITION ||
                msg->message == WM_IME_ENDCOMPOSITION ||
                msg->message == WM_IME_COMPOSITION ||
                msg->message == WM_IME_CHAR ||
                msg->message == WM_IME_REQUEST) {
                context->DispatchEvent(msg->message, msg->wParam, msg->lParam);
                return true;
            } else {
                // 其他事件派发到自绘引擎
                context->DispatchEvent(msg->message, msg->wParam, msg->lParam);
            }
        }
        return QWidget::nativeEvent(eventType, message, result);
    }
};


