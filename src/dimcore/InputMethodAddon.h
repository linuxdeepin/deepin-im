#ifndef INPUTMETHODADDON_H
#define INPUTMETHODADDON_H

#include "Addon.h"
#include "InputMethodEntry.h"

#include <QList>

namespace org {
namespace deepin {
namespace dim {

class InputMethodAddon : public Addon
{
    Q_OBJECT

public:
    explicit InputMethodAddon(Dim *dim, const QString &key);
    virtual ~InputMethodAddon();

    virtual QList<InputMethodEntry> getInputMethods() = 0;
    virtual void keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent) = 0;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTMETHODADDON_H
