#include "sourcecontroller.h"

SourceController::SourceController(QObject *parent) : QObject(parent) {
    registerSource(tr("Not selected"), nullptr);
}

bool SourceController::sourceExists(const QString &sourceName) {
    return m_sources.contains(sourceName);
}

bool SourceController::registerSource(const QString &name, Source *source) {
    if (sourceExists(name)) {
        return false;
    }

    if (source) {
        source->setParent(this);
    }

    m_sources.insert(name, source);

    return true;
}

QStringList SourceController::getSources() const {
    return m_sources.keys();
}
