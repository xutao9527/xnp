#include <QApplication>
#include <QVBoxLayout>
#include <QIcon>
#include <fcntl.h>
#include <io.h>
#include "XnpWinGLWidget.h"

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8); // 设置控制台输出为 UTF-8
    _setmode(_fileno(stdout), _O_U8TEXT); // 设置标准输出为 UTF-8
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    XnpWinGLWidget window;
    window.setWindowTitle(u8"游戏引擎");  // 设置窗口标题
    window.resize(512, 384); // 设置窗口大小
    window.show();
    return QApplication::exec();
}
