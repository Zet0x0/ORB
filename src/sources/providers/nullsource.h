#pragma once

#include "../source.h"
#include <QString>

class NullSource : public Source {
    Q_OBJECT

public:
    void cancelSearch() override;

    QString websiteUrl() const override;

private:
    void handleSearch(const QString &query) override;
};
