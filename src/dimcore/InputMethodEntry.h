#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QString>

#include <memory>

class InputMethodEntryPrivate;

namespace org {
namespace deepin {
namespace dim {

class InputMethodAddon;
class InputContextKeyEvent;

class InputMethodEntry
{
public:
    InputMethodEntry(const QString &addon,
                     const QString &uniqueName,
                     const QString &name,
                     const QString &description,
                     const QString &label,
                     const QString &iconName);
    ~InputMethodEntry();

    const QString &addon() const;
    const QString &uniqueName() const;
    const QString &name() const;
    const QString &description() const;
    const QString &label() const;
    const QString &iconName() const;

private:
    std::shared_ptr<InputMethodEntryPrivate> d;
};

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !INPUTMETHOD_H
