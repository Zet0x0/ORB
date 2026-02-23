#pragma once

#include "source.h"
#include <QNetworkRequestFactory>
#include <QRestAccessManager>

class RadioRecord : public Source {
    Q_OBJECT

private:
    QNetworkAccessManager *m_networkAccessManager = nullptr;
    QRestAccessManager *m_restAccessManager = nullptr;

    QNetworkRequestFactory m_api{
        {QStringLiteral("https://www.radiorecord.ru/api")}};

public:
    explicit RadioRecord();

    void search(const QString &query) override;
    void cancelSearch() override;

    bool hasDefaultStations() const override;
    void loadDefaultStations() override;
};
