#pragma once

#include "source.h"
#include <QNetworkRequestFactory>
#include <QRestAccessManager>

class RadioRecord : public Source {
    Q_OBJECT

private:
    struct Api {
        inline static const QString BaseUrl =
            QStringLiteral("https://www.radiorecord.ru/api");

        struct CacheExpiries {
            static const int Stations = 60 * 10; // 10 minutes
        }; // NOTE: in seconds

        struct Paths {
            inline static const QString Stations = QStringLiteral("/stations");
        };
    };

    struct DefaultStations {
    private:
        QList<Station> m_stations;
        qint64 m_cachedAt = -1;

        void resetCache();

    public:
        void setCached(const QList<Station> &stations);
        const QList<Station> &getCached();
    };

    QNetworkAccessManager *m_networkAccessManager = nullptr;
    QRestAccessManager *m_restAccessManager = nullptr;
    QNetworkRequestFactory m_api{{Api::BaseUrl}};

    DefaultStations m_defaultStations;

    bool parseJson(QRestReply &reply, QJsonDocument *json);

    void handleStationsEndpointResult(const QJsonDocument &json);

private slots:
    void onSearchRequestFinished(QRestReply &reply);

public:
    explicit RadioRecord();

    void search(const QString &query) override;
    void cancelSearch() override;

    bool hasDefaultStations() const override;
    void loadDefaultStations() override;
};
