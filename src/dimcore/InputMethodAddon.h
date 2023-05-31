#ifndef INPUTMETHODADDON_H
#define INPUTMETHODADDON_H

#include <QList>

#include "Addon.h"
#include "InputMethodEntry.h"

class InputMethodAddon : public Addon {
    Q_OBJECT

public:
    explicit InputMethodAddon(Dim *dim, const QString &key);
    virtual ~InputMethodAddon();

    virtual QList<InputMethodEntry> getInputMethods() = 0;
};

#endif // !INPUTMETHODADDON_H
