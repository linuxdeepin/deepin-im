#include "InputMethodEntry.h"

#include "InputMethodAddon.h"

class InputMethodEntryPrivate {
public:
    QString addon;
    QString uniqueName;
    QString name;
    QString description;
    QString label;
    QString iconName;
};

InputMethodEntry::InputMethodEntry(const QString &addon,
                                   const QString &uniqueName,
                                   const QString &name,
                                   const QString &description,
                                   const QString &label,
                                   const QString &iconName)
    : d(new InputMethodEntryPrivate{addon, uniqueName, name, description, label, iconName}) {
}

InputMethodEntry::~InputMethodEntry() {
}

const QString &InputMethodEntry::addon() const {
    return d->addon;
}

const QString &InputMethodEntry::uniqueName() const {
    return d->uniqueName;
}

const QString &InputMethodEntry::name() const {
    return d->name;
}

const QString &InputMethodEntry::description() const {
    return d->description;
}

const QString &InputMethodEntry::label() const {
    return d->label;
}

const QString &InputMethodEntry::iconName() const {
    return d->iconName;
}

void InputMethodEntry::processKeyEvent(KeyEvent &event) {
    Q_UNUSED(event);
}
