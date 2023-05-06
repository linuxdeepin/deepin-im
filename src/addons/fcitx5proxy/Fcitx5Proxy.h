#ifndef FCITX5PROXY_H
#define FCITX5PROXY_H

#include <dimcore/InputMethodAddon.h>

class Fcitx5Proxy : public InputMethodAddon {
public:
    Fcitx5Proxy(Dim *dim);
    QList<InputMethodEntry> getInputMethods() override;
};

#endif // !FCITX5PROXY_H
