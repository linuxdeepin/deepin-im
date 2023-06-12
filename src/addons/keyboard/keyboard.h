#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <dimcore/InputMethodAddon.h>
#include <xkbcommon/xkbcommon.h>

class QDomElement;

namespace org {
namespace deepin {
namespace dim {

template <auto FreeFunction>
struct FunctionDeleter {
    template <typename T>
    void operator()(T *p) const {
        if (p) {
            FreeFunction(const_cast<std::remove_const_t<T> *>(p));
        }
    }
};
template <typename T, auto FreeFunction = std::free>
using UniqueCPtr = std::unique_ptr<T, FunctionDeleter<FreeFunction>>;
static_assert(sizeof(char *) == sizeof(UniqueCPtr<char>),
              ""); // ensure no overhead

class Keyboard : public InputMethodAddon
{
public:
    Keyboard(Dim *dim);
    ~Keyboard();

    QList<InputMethodEntry> getInputMethods() override;
    void keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent) override;

private:
    UniqueCPtr<struct xkb_context, xkb_context_unref> ctx_;
    UniqueCPtr<struct xkb_keymap, xkb_keymap_unref> keymap_;
    UniqueCPtr<struct xkb_state, xkb_state_unref> state_;
    QList<InputMethodEntry> keyboards_;

    void parseRule(const QString &file);
    void parseLayoutList(const QDomElement &layoutListEle);
    void parseVariantList(const QString &layoutName, const QDomElement &variantListEle);
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !KEYBOARD_H
