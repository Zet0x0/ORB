#include "nullsource.h"

void NullSource::search(const QString &query) {
    Q_UNUSED(query)

    emit searchErrorOccurred(tr("This source is not searchable"));
}

void NullSource::cancelSearch() {
    emit searchCancelled();
}
