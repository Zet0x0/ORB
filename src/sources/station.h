#pragma once

#include <QQmlEngine>

struct Station {
    Q_GADGET
    QML_NAMED_ELEMENT(station)

    Q_PROPERTY(bool valid READ isValid FINAL)

    Q_PROPERTY(QString name READ name WRITE setName FINAL)
    Q_PROPERTY(QString streamUrl READ streamUrl WRITE setStreamUrl FINAL)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl FINAL)

private:
    bool m_valid = false;

    QString m_name;
    QString m_streamUrl;
    QString m_imageUrl;

    void setValid(bool newValid);

public:
    Station(const QString &name = QString(),
            const QString &streamUrl = QString(),
            const QString &imageUrl = QString());

    bool isValid() const;

    QString name() const;
    void setName(QString newName);
    QString streamUrl() const;
    void setStreamUrl(QString newStreamUrl);
    QString imageUrl() const;
    void setImageUrl(QString newImageUrl);

    QVariantMap toMap() const;
    static Station fromMap(const QVariantMap &map);

    bool operator==(const Station &other) const;
};
