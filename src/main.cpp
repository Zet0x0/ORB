#include "sources/providers/nullsource.h"
#include "sources/providers/radiorecord.h"
#include "sources/sourcecontroller.h"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName(QStringLiteral("zet0x0"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("com.zet0x0.orb"));
    QCoreApplication::setApplicationName(QStringLiteral("ORB"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));

    QGuiApplication::setQuitOnLastWindowClosed(false);

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QIcon::setThemeName("ORB");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Add sources
    {
        SourceController *sourceController = SourceController::instance();

        sourceController->registerSource(
            SourceControllerConstants::NullSourceKey.toString(),
            QObject::tr("Not selected"), new NullSource);
        sourceController->registerSource(QStringLiteral("radio-record"),
                                         QObject::tr("Radio Record"),
                                         new RadioRecord);
    }

    engine.loadFromModule(QStringLiteral("ORB"), QStringLiteral("Main"));

    return app.exec();
}
