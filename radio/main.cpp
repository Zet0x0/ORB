#include "sources/nullsource.h"
#include "sources/radiorecord.h"
#include "sources/sourcecontroller.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
    QQuickStyle::setFallbackStyle(QStringLiteral("Fusion"));

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QIcon::setThemeName("unnamed-radio-app");

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Add sources
    {
        SourceController *sourceController = SourceController::instance();

        sourceController->registerSource(QObject::tr("Not selected"),
                                         new NullSource);
        sourceController->registerSource(QObject::tr("Radio Record"),
                                         new RadioRecord);
    }

    engine.loadFromModule(QStringLiteral("radio"), QStringLiteral("Main"));

    return app.exec();
}
