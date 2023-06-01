#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QString>

#include <memory>

class InputMethodAddon;
class InputMethodEntryPrivate;
class KeyEvent;

class InputMethodEntry {
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

#endif // !INPUTMETHOD_H
