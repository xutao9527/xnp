#pragma once

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QFontDatabase>
#include <QSettings>

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
    explicit DImageWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        // 加载自定义字体
        int fontId = QFontDatabase::addApplicationFont("resources/ALIBABA-PUHUITI-HEAVY.TTF");
        if (fontId != -1) {
            QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
            if (!fontFamilies.isEmpty()) {
                // 使用加载的字体
                QFont font(fontFamilies.at(0), 30);
                setFont(font);
            }
        }
        QSettings settings("config.ini", QSettings::IniFormat);
        line1 = settings.value("DisplaySettings/line1").toString();
        line2 = settings.value("DisplaySettings/line2").toString();
        line3 = settings.value("DisplaySettings/line3").toString();
        line4 = settings.value("DisplaySettings/line4").toString();
        line5 = settings.value("DisplaySettings/line5").toString();
        // line1 = u8"台号: 089"; // 设置要显示的文本
        // line2 = u8"台面限红/Table Limits"; // 设置要显示的文本
        // line3 = u8"单骰/:MIN 200-MAX 16500"; // 设置要显示的文本
        // line4 = u8"双骰/:MIN 200-MAX 7100"; // 设置要显示的文本
        // line5 = u8"围骰/:MIN 200-MAX 2000"; // 设置要显示的文本
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

        painter.setFont(font());
        painter.setPen(QPen(QColor(119, 91, 37), 1)); // 设置轮廓颜色和宽度
        QRect textRect = painter.boundingRect(rect(), Qt::AlignCenter | Qt::TextWordWrap, line1);

        int offset = 3; // 偏移量，用于轮廓效果
        // 绘制文本
        painter.drawText((width() - textRect.width()) / 2, 15 + textRect.height(), line1);
        painter.drawText(20 - offset, 80 + textRect.height() + offset, line2);
        painter.drawText(20 - offset, 140 + textRect.height() + offset, line3);
        painter.drawText(20 - offset, 200 + textRect.height() + offset, line4);
        painter.drawText(20 - offset, 260 + textRect.height() + offset, line5);

        // 设置文本颜色
        painter.setPen(Qt::white);
        painter.drawText((width() - textRect.width()) / 2, 5 + textRect.height(),line1); // y + textRect.height() 用于在文字底部绘制
        painter.drawText(20, 80 + textRect.height(), line2);
        painter.drawText(20, 140 + textRect.height(), line3);
        painter.drawText(20, 200 + textRect.height(), line4);
        painter.drawText(20, 260 + textRect.height(), line5);
    }
};

