﻿#include <QQmlApplicationEngine>
#include <QGuiApplication>


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/ui/login.qml")));
    return app.exec();
}