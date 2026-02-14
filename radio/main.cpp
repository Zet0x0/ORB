#include "player/player.h"
#include "sources/sourcecontroller.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#define REGISTER_QML_SINGLETON(type, uri)                                  \
    qmlRegisterSingletonInstance<type>(uri, 1, 0, #type, type::instance())

int main(int argc, char *argv[]) {
    QQuickStyle::setStyle(QStringLiteral("Fusion"));

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // TODO: remove this useless thing
    REGISTER_QML_SINGLETON(Player, "radio.player");
    REGISTER_QML_SINGLETON(SourceController, "radio.sources");

    // TODO
    // SourceController *sourceController = SourceController::instance();
    // sourceController->registerSource("testsource1", new Source);
    // sourceController->registerSource("testsource1", new Source);
    // sourceController->registerSource("testsource2", new Source);

    engine.loadFromModule(QStringLiteral("radio"), QStringLiteral("Main"));

    return app.exec();
}
