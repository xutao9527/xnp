#pragma once


#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include "OpenGLWidget.h"


class XnpQWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    std::shared_ptr<OpenGLWidget> openGlWidget;

public:
    explicit XnpQWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {
        // // 创建并设置 OpenGLWidget 作为 XnpQWidget 的子部件
        // openGlWidget = std::make_shared<OpenGLWidget>(this);
        //
        // // 设置布局
        // auto *layout = new QVBoxLayout(this);
        // //layout->addWidget(inputField);
        // layout->addWidget(openGlWidget.get());
        //
        // // 设置布局的边距和间距为 0
        // layout->setContentsMargins(0, 0, 0, 0);
        // layout->setSpacing(0);
        // // 设置布局的伸缩因子
        // layout->setStretch(0, 1); // 设置 OpenGLWidget 伸缩因子为 1
        // setLayout(layout);
        // // 调整大小
        // adjustSize();
    }

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override {

        // MSG *msg = static_cast<MSG *>(message);
        // if (openGlWidget) {
        //     openGlWidget->DispatchEvent(msg);
        //     if (msg->message == WM_IME_STARTCOMPOSITION ||
        //         msg->message == WM_IME_ENDCOMPOSITION ||
        //         msg->message == WM_IME_COMPOSITION ||
        //         msg->message == WM_IME_CHAR ||
        //         msg->message == WM_IME_REQUEST) {
        //         qDebug() << "XnpQWidget:" << std::hash<std::thread::id>{}(std::this_thread::get_id()) << ",WM_IME_" << msg->message;
        //         return true;
        //     }
        // }
        return QWidget::nativeEvent(eventType, message, result);
    }
};

