#include "station.h"
#include "../common/utilities.h"

void Station::setValid(bool newValid) {
    m_valid = newValid;
}

Station::Station(const QString &name, const QString &streamUrl,
                 const QString &imageUrl) {
    setName(name);
    setStreamUrl(streamUrl);
    setImageUrl(imageUrl);
}

bool Station::isValid() const {
    return m_valid;
}

QString Station::name() const {
    return m_name;
}

void Station::setName(QString newName) {
    newName = newName.trimmed();

    if (newName.isEmpty()) {
        newName = QObject::tr("Unnamed Station");
    }

    m_name = Utilities::escapeControlCharacters(newName);
}

QString Station::streamUrl() const {
    return m_streamUrl;
}

void Station::setStreamUrl(QString newStreamUrl) {
    m_streamUrl = Utilities::normalizeUserInputUrl(newStreamUrl);

    setValid(!m_streamUrl.isEmpty());
}

QString Station::imageUrl() const {
    return m_imageUrl;
}

void Station::setImageUrl(QString newImageUrl) {
    m_imageUrl = Utilities::normalizeUserInputUrl(newImageUrl);
}

QVariantMap Station::toMap() const {
    return {{QStringLiteral("name"), m_name},
            {QStringLiteral("streamUrl"), m_streamUrl},
            {QStringLiteral("imageUrl"), m_imageUrl}};
}

Station Station::fromMap(const QVariantMap &map) {
    return Station(map.value(QStringLiteral("name")).toString(),
                   map.value(QStringLiteral("streamUrl")).toString(),
                   map.value(QStringLiteral("imageUrl")).toString());
}

bool Station::operator==(const Station &other) const {
    return this->name() == other.name() &&
           this->streamUrl() == other.streamUrl() &&
           this->imageUrl() == other.imageUrl();
}
