#pragma once

#include "../source.h"
#include <QString>

class NullSource : public Source {
    Q_OBJECT

private:
    void handleSearch(const QString &query) override;

public:
    void cancelSearch() override;

    QString websiteUrl() const override;
};
