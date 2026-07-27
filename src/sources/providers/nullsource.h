#pragma once

#include "../source.h"

class NullSource : public Source {
    Q_OBJECT

private:
    void handleSearch(const QString &query) override;

public:
    void cancelSearch() override;

    QString websiteUrl() const override;
};
