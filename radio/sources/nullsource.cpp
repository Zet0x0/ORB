#include "nullsource.h"

void NullSource::handleSearch(const QString &query) {
    Q_UNUSED(query)

    raiseError(tr("Search error"), tr("This source is not searchable"));
}

void NullSource::cancelSearch() {}
