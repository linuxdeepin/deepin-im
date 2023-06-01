#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <QString>

class Dim;
class InputMethodAddon;
class InputMethodEntry;

class InputState {
public:
    InputState(Dim *dim);

    const QString &currentIM() const;

private:
    Dim *dim_;
    QString current_im_;
};

#endif // !INPUTSTATE_H
