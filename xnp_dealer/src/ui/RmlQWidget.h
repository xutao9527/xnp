#pragma once

#include <QWidget>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <windows.h>
#include "DbgRenderer.h"

class RmlQWidget : public QWidget
{
    std::shared_ptr<DbgRenderer> rendererContext;
public:
    explicit RmlQWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        qDebug() << "RmlQWidget";
    }

    ~RmlQWidget() override
    {
        qDebug() << "~RmlQWidget";
    }

protected:
    void showEvent(QShowEvent *event) override
    {
        QWidget::showEvent(event);
        HWND hwnd = reinterpret_cast<HWND>(this->winId());
        QString title = this->windowTitle();
        QRect contents = this->contentsRect();
        int clientWidth = contents.width();
        int clientHeight = contents.height();
        rendererContext = std::make_shared<DbgRenderer>(hwnd, title.toStdString(), clientWidth, clientHeight);
        rendererContext->Run();
    }

    void closeEvent(QCloseEvent *event) override
    {
        QWidget::closeEvent(event);
    }

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override
    {
        MSG *msg = static_cast<MSG *>(message);
        if (auto context = rendererContext) {
            // 文本框输出,激活输入法动态候选框位置
            if (msg->message == WM_IME_COMPOSITION) {
                context->ActivateKeyboard();
            }
            //屏蔽IME事件
            if (msg->message == WM_IME_STARTCOMPOSITION ||
                msg->message == WM_IME_ENDCOMPOSITION ||
                msg->message == WM_IME_COMPOSITION ||
                msg->message == WM_IME_CHAR ||
                msg->message == WM_IME_REQUEST) {
                // 不做任何处理
            } else {
                // 派发事件到自绘引擎
                context->DispatchEvent(msg->message, msg->wParam, msg->lParam);
            }
        }
        return QWidget::nativeEvent(eventType, message, result);
    }
};


