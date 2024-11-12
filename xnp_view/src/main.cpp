#include <QApplication>
#include "ui/FcruQWidget.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    FcruQWidget fcruQWidget;

    fcruQWidget.setWindowTitle(u8"鱼虾蟹");  // 设置窗口标题
    fcruQWidget.resize(1600, 900); // 设置窗口大小
    fcruQWidget.show();
    return app.exec();
}