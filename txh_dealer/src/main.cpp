#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QDir>
#include <QDebug>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 创建主窗口
    QWidget window;
    window.setWindowTitle("Example");
    window.resize(400, 300); // 设置窗口大小

    // 窗口图标
    QIcon icon(":/resources/icon.ico");
    if (icon.isNull()) {
        qDebug() << "Icon is null!";
    } else {
        window.setWindowIcon(icon);
    }

    //窗口图片
    QPixmap pixmap(":/resources/demo.png");
    QLabel *iconLabel = new QLabel;
    iconLabel->setPixmap(pixmap);
    iconLabel->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(iconLabel, 0, Qt::AlignCenter); // 将 QLabel 居中

    window.setLayout(layout);
    window.show();
    return app.exec();
}
