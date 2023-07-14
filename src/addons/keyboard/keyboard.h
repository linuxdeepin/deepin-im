#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common/common.h"

#include <dimcore/InputMethodAddon.h>
#include <xkbcommon/xkbcommon.h>

class QDomElement;

namespace org {
namespace deepin {
namespace dim {

class Keyboard : public InputMethodAddon
{
public:
    Keyboard(Dim *dim);
    ~Keyboard();

    QList<InputMethodEntry> getInputMethods() override;
    bool keyEvent(const InputMethodEntry &entry, InputContextKeyEvent &keyEvent) override;

private:
    std::unique_ptr<struct xkb_context, Deleter<xkb_context_unref>> ctx_;
    std::unique_ptr<struct xkb_keymap, Deleter<xkb_keymap_unref>> keymap_;
    std::unique_ptr<struct xkb_state, Deleter<xkb_state_unref>> state_;
    QList<InputMethodEntry> keyboards_;

    void parseRule(const QString &file);
    void parseLayoutList(const QDomElement &layoutListEle);
    void parseVariantList(const QString &layoutName, const QDomElement &variantListEle);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !KEYBOARD_H
