#pragma once

#include "source.h"
#include <QNetworkRequestFactory>

namespace RadioRecordConstants {
const QString BaseApiUrl = QStringLiteral("https://www.radiorecord.ru/api");

const QString DefaultStationsPath = QStringLiteral("/stations/");
const QString SearchPath = QStringLiteral("/search/");
}

class RadioRecord : public Source {
    Q_OBJECT

private:
    QNetworkRequestFactory m_api{{RadioRecordConstants::BaseApiUrl}};

    QNetworkReply *m_runningReply = nullptr;

    QJsonArray extractStationsFromJson(const QJsonDocument &json) const;

    void processStationIntoList(const QJsonObject &rawStation,
                                QList<Station> *stations) const;

    void handleSearch(const QString &query) override;

    void handleLoadDefaultStations() override;

    void handleStationsEndpointResult(const QJsonDocument &json);
    void handleSearchEndpointResult(const QJsonDocument &json);

private slots:
    void onSearchRequestFinished(QRestReply &reply);

public:
    void cancelSearch() override;

    bool hasDefaultStations() const override;
};
