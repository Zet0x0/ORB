#pragma once

#include "../source.h"
#include <QNetworkRequestFactory>

namespace RadioRecordConstants {
constexpr QLatin1StringView WebsiteUrl("https://www.radiorecord.ru/");

constexpr QLatin1StringView BaseApiUrl("https://www.radiorecord.ru/api");

constexpr QLatin1StringView DefaultStationsPath("/stations/");
constexpr QLatin1StringView SearchPath("/search/");
}

class RadioRecord : public Source {
    Q_OBJECT

private:
    QNetworkRequestFactory m_api{{RadioRecordConstants::BaseApiUrl}};

    QPointer<QNetworkReply> m_runningReply;

    QJsonArray extractStationsFromJson(const QJsonDocument &json) const;

    void processStationIntoList(const QJsonObject &rawStation,
                                QList<Station> *stations) const;

    void handleSearch(const QString &query) override;

    void handleLoadDefaultStations() override;

    void handleStationsEndpointResult(const QJsonDocument &json);
    void handleSearchEndpointResult(const QJsonDocument &json);

    bool finishReply(QRestReply &reply, QJsonDocument *json);

private slots:
    void onSearchRequestFinished(QRestReply &reply);
    void onDefaultStationsRequestFinished(QRestReply &reply);

public:
    void cancelSearch() override;

    bool hasDefaultStations() const override;

    QString websiteUrl() const override;
};
