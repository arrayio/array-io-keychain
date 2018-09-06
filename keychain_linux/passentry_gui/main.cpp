#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qtwebengineglobal.h>

#include "controller.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QtWebEngine::initialize();

    //Controller controller;

    qmlRegisterType<Controller>("Controller", 1, 0, "Controller");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //engine.rootContext()->setContextProperty("controller", &controller);
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
