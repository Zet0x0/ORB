#pragma once

#include "source.h"

class NullSource : public Source {
    Q_OBJECT

public:
    void search(const QString &query) override;
};
