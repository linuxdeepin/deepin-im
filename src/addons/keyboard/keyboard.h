#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <dimcore/InputMethodAddon.h>

struct xkb_context;

class Keyboard : public InputMethodAddon {
public:
    Keyboard(Dim *dim);
    ~Keyboard();

    QList<InputMethodEntry> getInputMethods() override;

private:
    xkb_context *ctx_;

    void parseRule(const QString &file);
};

#endif // !KEYBOARD_H
