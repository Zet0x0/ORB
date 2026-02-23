#include "radiorecord.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRestAccessManager>
#include <QRestReply>

RadioRecord::RadioRecord()
    : m_networkAccessManager(new QNetworkAccessManager(this)),
      m_restAccessManager(
          new QRestAccessManager(m_networkAccessManager, this)) {}

// TODO
void RadioRecord::search(const QString &query) {}

// TODO
void RadioRecord::cancelSearch() {}

bool RadioRecord::hasDefaultStations() const {
    return true;
}

// TODO
void RadioRecord::loadDefaultStations() {
    emit searchStarted();

    QNetworkReply *reply = m_restAccessManager->get(
        m_api.createRequest(QStringLiteral("/stations")), this,
        [this](QRestReply &reply) {
            if (!reply.isSuccess()) {
                emit errorOccurred(
                    SourceError{tr("Search Error"), reply.errorString()});

                return;
            }

            QJsonParseError jsonError;
            const std::optional<QJsonDocument> jsonDocument =
                reply.readJson(&jsonError);

            if (jsonError.error != QJsonParseError::NoError) {
                emit errorOccurred(
                    SourceError{tr("Parse Error"),
                                tr("%0 (offset %1)")
                                    .arg(jsonError.errorString(),
                                         QString::number(jsonError.offset))});

                return;
            }

            const QJsonArray rawStations =
                jsonDocument->object()
                    .value(QStringLiteral("result"))
                    .toObject()
                    .value(QStringLiteral("stations"))
                    .toArray();
            QList<Station> stations;

            for (const QJsonValue &rawStationValue : rawStations) {
                const QJsonObject rawStation = rawStationValue.toObject();

                if (rawStation.isEmpty()) {
                    continue;
                }

                Station station{
                    rawStation.value(QStringLiteral("title")).toString(),
                    rawStation.value(QStringLiteral("stream_hls")).toString(),
                    rawStation.value(QStringLiteral("icon_gray")).toString()};

                if (station.isValid()) {
                    stations << station;
                }
            }

            if (stations.isEmpty()) {
                emit errorOccurred(SourceError{
                    tr("Search Error"), tr("No default stations found")});

                return;
            }

            emit stationsDispatched(stations);
        });
}
