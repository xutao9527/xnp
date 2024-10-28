#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include "BImageWidget.h"
#include "DImageWidget.h"

class FcruQWidget : public QWidget
{
Q_OBJECT
private:
    QStackedLayout  *layout;
    BImageWidget *bImageWidget;
    DImageWidget *dImageWidget;
public:
    FcruQWidget() {
        // layout = new QStackedLayout (this);
        // layout->setContentsMargins(0, 0, 0, 0);
        // layout->setSpacing(0);

        bImageWidget  = new BImageWidget(this);
        bImageWidget->setImage("resources/index.jpg");
        //layout->addWidget(bImageWidget); // 将自定义控件添加到布局

        dImageWidget  = new DImageWidget(this);
        dImageWidget->setImage("resources/describe.png");

        //layout->addWidget(dImageWidget); // 将自定义控件添加到布局

        // setLayout(layout);
    }

protected:

    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        // 设置背景图占满整个区域
        bImageWidget->setGeometry(rect());

        // 设置 dImageWidget 的位置
        dImageWidget->setGeometry(34, 15, 583, 341); // 坐标在 (50, 50)，大小为 300x300
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_F8) {
            toggleFullScreen();
        }
        QWidget::keyPressEvent(event);
    }

private slots:
    void toggleFullScreen() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    }
};



