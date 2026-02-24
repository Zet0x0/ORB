#pragma once

#include "source.h"
#include <QNetworkRequestFactory>

namespace RadioRecordConstants {
const QString BaseApiUrl = QStringLiteral("https://www.radiorecord.ru/api");

const QString StationsPath = QStringLiteral("/stations");
}

class RadioRecord : public Source {
    Q_OBJECT

private:
    QNetworkRequestFactory m_api{{RadioRecordConstants::BaseApiUrl}};

    void handleLoadDefaultStations() override;

    void handleStationsEndpointResult(const QJsonDocument &json);

private slots:
    void onSearchRequestFinished(QRestReply &reply);

public:
    void search(const QString &query) override;
    void cancelSearch() override;

    bool hasDefaultStations() const override;
};
