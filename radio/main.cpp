#include "player/player.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#define REGISTER_QML_SINGLETON(type, uri)                                  \
    qmlRegisterSingletonInstance<type>(uri, 1, 0, #type, type::instance())

int main(int argc, char *argv[]) {
    QQuickStyle::setStyle("Fusion");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    REGISTER_QML_SINGLETON(Player, "radio.player");

    engine.loadFromModule("radio", "Main");

    return app.exec();
}
