#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

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

    // TODO
    // SourceController *sourceController = SourceController::instance();
    // sourceController->registerSource("testsource1", new Source);
    // sourceController->registerSource("testsource1", new Source);
    // sourceController->registerSource("testsource2", new Source);

    engine.loadFromModule(QStringLiteral("radio"), QStringLiteral("Main"));

    return app.exec();
}
