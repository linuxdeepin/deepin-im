#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QString>

#include <memory>

class InputMethodEntryPrivate;
class InputMethodEntry {
public:
    InputMethodEntry(const QString &uniqueName,
                     const QString &name,
                     const QString &description,
                     const QString &label,
                     const QString &iconName);
    
    const QString &uniqueName () const;
    const QString &name () const;
    const QString &description () const;
    const QString &label () const;
    const QString &iconName () const;

private:
    std::unique_ptr<InputMethodEntryPrivate> d;
};

#endif // !INPUTMETHOD_H
