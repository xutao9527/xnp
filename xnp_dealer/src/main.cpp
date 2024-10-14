#include <QApplication>
#include <QVBoxLayout>
#include <QIcon>
#include "ui/RmlQWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RmlQWidget window;
    window.setWindowTitle("游戏引擎");  // 设置窗口标题
    window.resize(512, 384); // 设置窗口大小
    window.show();

    return QApplication::exec();
}
