#include "sources/providers/nullsource.h"
#include "sources/providers/radiorecord.h"
#include "sources/sourcecontroller.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
    QQuickStyle::setFallbackStyle(QStringLiteral("Fusion"));

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QIcon::setThemeName("orb");

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Add sources
    {
        SourceController *sourceController = SourceController::instance();

        sourceController->registerSource(QStringLiteral("none"),
                                         QObject::tr("Not selected"),
                                         new NullSource);
        sourceController->registerSource(QStringLiteral("radio-record"),
                                         QObject::tr("Radio Record"),
                                         new RadioRecord);
    }

    engine.loadFromModule(QStringLiteral("orb"), QStringLiteral("Main"));

    return app.exec();
}
