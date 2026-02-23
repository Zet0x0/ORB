#include "source.h"

void Source::raiseError(const QString &title, const QString &message) {
    SourceError error;

    error.title = title;
    error.message = message;

    emit errorOccurred(error);
}

Source::Source(QObject *parent) : QObject(parent) {}

bool Source::hasDefaultStations() const {
    return false;
}

void Source::loadDefaultStations() {}
