#pragma once

#include <QWidget>
#include <QPainter>
#include <QPixmap>

class DImageWidget : public QWidget
{
Q_OBJECT
private:
    QPixmap pixmap; // 存储要显示的图片
    QString line1;   // 要显示的文本
    QString line2;   // 要显示的文本
    QString line3;   // 要显示的文本
    QString line4;   // 要显示的文本
    QString line5;   // 要显示的文本
public:
    explicit DImageWidget(QWidget *parent = nullptr) : QWidget(parent) {
        line1 = "台号: 089"; // 设置要显示的文本
        line2 = "台面限红/Table Limits"; // 设置要显示的文本
        line3 = "单骰/:MIN 200-MAX 16500"; // 设置要显示的文本
        line4 = "双骰/:MIN 200-MAX 7100"; // 设置要显示的文本
        line5 = "围骰/:MIN 200-MAX 2000"; // 设置要显示的文本
    }

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

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 32));

        QRect textRect = painter.boundingRect(rect(), Qt::AlignCenter | Qt::TextWordWrap, line1);

        painter.drawText((width() - textRect.width()) / 2, 15 + textRect.height(), line1); // y + textRect.height() 用于在文字底部绘制

        painter.drawText(20, 90 + textRect.height(), line2);

        painter.drawText(20, 150 + textRect.height(), line3);

        painter.drawText(20, 210 + textRect.height(), line4);

        painter.drawText(20, 270 + textRect.height(), line5);

        // painter.drawText((width() - textRect.width()) / 2, 20 + textRect.height(), line1); // y + textRect.height() 用于在文字底部绘制
        //
        // painter.drawText((width() - textRect.width()) / 2, 20 + textRect.height(), line1); // y + textRect.height() 用于在文字底部绘制
        //
        // painter.drawText((width() - textRect.width()) / 2, 20 + textRect.height(), line1); // y + textRect.height() 用于在文字底部绘制
    }
};

