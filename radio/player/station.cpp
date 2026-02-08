#include "station.h"

QString Station::qUrlParse(const QString &rawUrl) {
    return QUrl::fromUserInput(rawUrl).toString();
}

Station::Station(QObject *parent, const QString &name, const QString &streamUrl,
                 const QString &imageUrl)
    : QObject(parent) {
    setName(name);
    setStreamUrl(streamUrl);
    setImageUrl(imageUrl);
}

Station::Station(const QString &name, const QString &streamUrl,
                 const QString &imageUrl)
    : Station(nullptr, name, streamUrl, imageUrl) {}

bool Station::isValid() const {
    return m_valid;
}

void Station::setValid(bool newValid) {
    if (m_valid == newValid) {
        return;
    }

    m_valid = newValid;

    emit validChanged();
}

QString Station::name() const {
    return m_name;
}

void Station::setName(QString newName) {
    newName = newName.trimmed();

    if (newName.isEmpty()) {
        newName = tr("Unnamed Station");
    }

    if (m_name == newName) {
        return;
    }

    m_name = newName;

    emit nameChanged();
}

QString Station::streamUrl() const {
    return m_streamUrl;
}

void Station::setStreamUrl(QString newStreamUrl) {
    newStreamUrl = qUrlParse(newStreamUrl);

    if (m_streamUrl == newStreamUrl) {
        return;
    }

    m_streamUrl = newStreamUrl;

    emit streamUrlChanged();

    setValid(!m_streamUrl.isEmpty());
}

QString Station::imageUrl() const {
    return m_imageUrl;
}

void Station::setImageUrl(QString newImageUrl) {
    newImageUrl = qUrlParse(newImageUrl);

    if (m_imageUrl == newImageUrl) {
        return;
    }

    m_imageUrl = newImageUrl;

    emit imageUrlChanged();
}

bool Station::operator==(const Station &other) const {
    return (this->name() == other.name() &&
            this->streamUrl() == other.streamUrl() &&
            this->imageUrl() == other.imageUrl());
}
