#include "sourcecontroller.h"

SourceController::SourceController(QObject *parent) : QObject(parent) {}

bool SourceController::sourceExists(const QString &sourceName) {
    return m_sources.contains(sourceName);
}

bool SourceController::registerSource(const QString &name, Source *source) {
    if (sourceExists(name)) {
        return false;
    }

    m_sources.insert(name, source);

    return true;
}
