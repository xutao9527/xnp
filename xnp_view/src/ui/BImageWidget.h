#pragma once

#include <QWidget>
#include <QPainter>
#include <QPixmap>

class BImageWidget : public QWidget
{
Q_OBJECT
private:
    QPixmap pixmap; // 存储要显示的图片

public:
    explicit BImageWidget(QWidget *parent = nullptr) : QWidget(parent) {}

    void setImage(const QString &path)
    {
        pixmap.load(path); // 从文件加载图片
        update(); // 触发重绘事件
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);
        QPainter painter(this);
        // 将图片缩放到控件大小
        if (!pixmap.isNull()) {
            painter.drawPixmap(rect(), pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
};



