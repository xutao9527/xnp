#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedLayout>
#include <QMenu>
#include "BImageWidget.h"
#include "DImageWidget.h"
#include <QApplication>
#include <QTimer>

class FcruQWidget : public QWidget
{
Q_OBJECT
private:

    BImageWidget *bImageWidget;
    DImageWidget *dImageWidget;
public:
    FcruQWidget() {
        bImageWidget  = new BImageWidget(this);
        bImageWidget->setImage("resources/index.jpg");

        dImageWidget  = new DImageWidget(this);
        dImageWidget->setImage("resources/describe.png");
        // 使用 QTimer 延迟全屏显示
        QTimer::singleShot(0, this, &FcruQWidget::showFullScreen);
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

    void contextMenuEvent(QContextMenuEvent *event) override {
        QMenu menu(this);
        QAction *exitAction = menu.addAction(u8"退出");
        connect(exitAction, &QAction::triggered, this, &FcruQWidget::quitApplication);
        menu.exec(event->globalPos());
    }

private slots:
    void toggleFullScreen() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    }

    void quitApplication() {
        QApplication::quit();
    }
};



