#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <QString>

namespace org {
namespace deepin {
namespace dim {

class Dim;
class InputMethodAddon;
class InputMethodEntry;

class InputState
{
public:
    InputState(Dim *dim);

    const QString &currentIM() const;

private:
    Dim *dim_;
    QString current_im_;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTSTATE_H
