#include <QApplication>
#include <QWidget>


int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QWidget widget;
    widget.setWindowTitle(u8"鱼虾蟹");  // 设置窗口标题
    widget.resize(1600, 900); // 设置窗口大小
    widget.show();
    return app.exec();
}