#ifndef INPUTMETHODADDON_H
#define INPUTMETHODADDON_H

#include <QList>

#include "Addon.h"
#include "InputMethodEntry.h"

class InputMethodAddon : public Addon {
public:
    explicit InputMethodAddon(Dim *dim);
    virtual QList<InputMethodEntry> getInputMethods() = 0;
};

#endif // !INPUTMETHODADDON_H
