#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QDir>
#include <QDebug>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 打印当前工作目录，调试图标路径问题
    qDebug() << "Current working directory:" << QDir::currentPath();

    // 创建主窗口
    QWidget window;
    window.setWindowTitle("Centered Text Example");
    window.resize(400, 300); // 设置窗口大小

    window.setWindowIcon(QIcon("resources/icon.ico"));

    // 创建一个 QLabel
    QLabel *label = new QLabel("Hello, Qt!阿");

    // 创建一个垂直布局并将 QLabel 添加到其中
    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(label); // 添加 QLabel 到布局

    // 设置布局
    layout->setAlignment(label, Qt::AlignCenter); // 让 QLabel 在布局中居中

    // 显示窗口
    window.setLayout(layout);
    window.show();

    return app.exec();
}
