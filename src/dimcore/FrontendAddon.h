#ifndef FRONTENDADDON_H
#define FRONTENDADDON_H

#include "Addon.h"

class FrontendAddon : public Addon {
    Q_OBJECT

public:
    explicit FrontendAddon(Dim *dim, const QString &key);
};

#endif // !FRONTENDADDON_H
