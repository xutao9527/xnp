#pragma once

#include <QWidget>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <windows.h>

class RmlQWidget : public QWidget
{
public:
    explicit RmlQWidget(QWidget *parent = nullptr) : QWidget(parent) {

    }

protected:
    void showEvent(QShowEvent *event) override
    {
        QWidget::showEvent(event);
        // 获取窗口 HWND (winId() 返回的是 HWND)
        HWND hwnd = reinterpret_cast<HWND>(this->winId());
        qDebug() << "窗口 HWND:" << hwnd;
        // 获取窗口标题
        QString title = this->windowTitle();
        qDebug() << "窗口标题:" << title;
        // 获取客户区的宽度和高度
        QRect contents = this->contentsRect();
        int clientWidth = contents.width();
        int clientHeight = contents.height();
        qDebug() << "客户区宽度:" << clientWidth << "客户区高度:" << clientHeight;
    }

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override{
        MSG *msg = static_cast<MSG *>(message);
        // 输出原生事件信息
        // qDebug() << "Received native event:"
        //          << "Message:" << msg->message
        //          << "wParam:" << msg->wParam
        //          << "lParam:" << msg->lParam;
        return QWidget::nativeEvent(eventType, message, result);
    }

};


