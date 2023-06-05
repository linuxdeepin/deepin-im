#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <dimcore/InputMethodAddon.h>

struct xkb_context;

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
    void keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent) override;

private:
    xkb_context *ctx_;
    QList<InputMethodEntry> keyboards_;

    void parseRule(const QString &file);
    void parseLayoutList(const QDomElement &layoutListEle);
    void parseVariantList(const QString &layoutName, const QDomElement &variantListEle);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !KEYBOARD_H
