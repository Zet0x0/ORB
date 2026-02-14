#include "nullsource.h"

void NullSource::search(const QString &query) {
    emit searchStarted();

    emit searchErrorOccurred(tr("This source is not searchable"));
}

void NullSource::cancelSearch() {
    emit searchCancelled();
}
