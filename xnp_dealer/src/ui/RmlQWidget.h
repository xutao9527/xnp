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
    QLineEdit *inputLineEdit;  // 输入框
public:
    explicit RmlQWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        //初始化输入框
        // inputLineEdit = new QLineEdit(this);
        // inputLineEdit->setPlaceholderText("请输入文本..."); // 设置占位符
        // inputLineEdit->setGeometry(10, 10, 300, 30); // 设置位置和大小
        qDebug() << "RmlQWidget";
    }

    ~RmlQWidget() override
    {
        qDebug() << "~RmlQWidget";
    }

protected:
    void showEvent(QShowEvent *event) override
    {
        rendererContext = std::make_shared<DbgRenderer>( reinterpret_cast<HWND>(this->winId()), this->windowTitle().toStdString(), this->contentsRect().width(), this->contentsRect().height());
        rendererContext->Run();
        QWidget::showEvent(event);
    }

    void closeEvent(QCloseEvent *event) override
    {
        rendererContext->WaitStop();        //等待渲染引擎先退出,释放资源
        QWidget::closeEvent(event);         // 继续处理关闭事件
    }

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override
    {
        MSG *msg = static_cast<MSG *>(message);
        if(msg->message == WM_CLOSE){
            qDebug() << "`````````````````````WM_CLOSE";
        }
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
                context->DispatchEvent(msg->message, msg->wParam, msg->lParam);
            } else {
                // 派发事件到自绘引擎
                context->DispatchEvent(msg->message, msg->wParam, msg->lParam);
            }
            //->DispatchEvent(msg->message, msg->wParam, msg->lParam);
        }
        return QWidget::nativeEvent(eventType, message, result);
    }
};


