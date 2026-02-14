#pragma once

#include "source.h"

class NullSource : public Source {
public:
    void search(const QString &query) override;
    void cancelSearch() override;
};
