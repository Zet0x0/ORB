#include "source.h"

Source::Source(QObject *parent) : QObject(parent) {}

bool Source::hasDefaultStations() {
    return false;
}

void Source::loadDefaultStations() {}
