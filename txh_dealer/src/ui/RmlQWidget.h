#pragma once

#include <QWidget>
#include <QDebug>

class RmlQWidget : public QWidget
{
public:
    explicit RmlQWidget(QWidget *parent = nullptr) : QWidget(parent) {

    }

protected:
    void showEvent(QShowEvent *event) override
    {
        QWidget::showEvent(event);

        // 获取窗口的宽度和高度
        int width = this->width();
        int height = this->height();

        qDebug() << "窗口宽度:" << width << "高度:" << height;
    }
};


