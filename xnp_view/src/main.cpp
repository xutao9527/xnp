#include <QApplication>
#include <QVBoxLayout>
#include <QIcon>
#include <QWidget>
#include <fcntl.h>
#include <io.h>


int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle(u8"游戏引擎");  // 设置窗口标题
    window.resize(512, 384); // 设置窗口大小
    window.show();
    return app.exec();
}