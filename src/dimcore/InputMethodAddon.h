#ifndef INPUTMETHODADDON_H
#define INPUTMETHODADDON_H

#include "Addon.h"
#include "InputMethodEntry.h"

#include <QList>

class InputMethodAddon : public Addon
{
    Q_OBJECT

public:
    explicit InputMethodAddon(Dim *dim, const QString &key);
    virtual ~InputMethodAddon();

    virtual QList<InputMethodEntry> getInputMethods() = 0;
    virtual void keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent) = 0;

Q_SIGNALS:
    void createInputContext(const QString &appName);
};

#endif // !INPUTMETHODADDON_H
