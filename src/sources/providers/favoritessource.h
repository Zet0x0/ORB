#pragma once

#include "../source.h"

class FavoritesSource : public Source {
    Q_OBJECT

private:
    QString m_query;

    QList<Station> filtered() const;
    void dispatch();

    void handleSearch(const QString &query) override;
    void handleLoadDefaultStations() override;

public:
    explicit FavoritesSource();

    void cancelSearch() override;

    bool hasDefaultStations() const override;

    QString websiteUrl() const override;
};
