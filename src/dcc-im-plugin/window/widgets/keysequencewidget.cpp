// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#include "keysequencewidget.h"

#include "qtkeytrans.h"
#include "utils/translate.h"

#include <QAction>
#include <QApplication>
#include <QHBoxLayout>
#include <QHash>
#include <QKeyEvent>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QTimer>
#include <QToolButton>

using namespace utilFuncs;

namespace dcc_im_plugin {
namespace widgets {

bool isX11LikePlatform()
{
    return QGuiApplication::platformName() == "xcb"
        || QGuiApplication::platformName().startsWith("wayland");
}

bool keyQtToFcitx(int keyQt, const QString &text, ModifierSide side, Key &outkey)
{
    int key = keyQt & (~Qt::KeyboardModifierMask);
    int state = keyQt & Qt::KeyboardModifierMask;
    int sym;
    unsigned int states;
    if (!keyQtToSym(key, Qt::KeyboardModifiers(state), text, sym, states)) {
        return false;
    }
    if (side == MS_Right) {
        switch (sym) {
        case FcitxKey_Control_L:
            sym = FcitxKey_Control_R;
            break;
        case FcitxKey_Alt_L:
            sym = FcitxKey_Alt_R;
            break;
        case FcitxKey_Shift_L:
            sym = FcitxKey_Shift_R;
            break;
        case FcitxKey_Super_L:
            sym = FcitxKey_Super_R;
            break;
        }
    }

    outkey = Key(static_cast<KeySym>(sym), KeyStates(states));
    return true;
}

class KeySequenceWidgetPrivate;

class KeySequenceButton : public QPushButton
{
    Q_OBJECT

public:
    explicit KeySequenceButton(KeySequenceWidgetPrivate *d, QWidget *parent)
        : QPushButton(parent)
        , d(d)
    {
    }

    virtual ~KeySequenceButton();

protected:
    /**
     * Reimplemented for internal reasons.
     */
    bool event(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    KeySequenceWidgetPrivate *const d;
};

class KeySequenceWidgetPrivate
{
public:
    KeySequenceWidgetPrivate(KeySequenceWidget *q);

    void init();

    static bool isOkWhenModifierless(int keyQt);

    void updateShortcutDisplay();
    void startRecording();

    void controlModifierlessTimout()
    {
        if (keySequence_.size() != 0 && !modifierKeys_) {
            // No modifier key pressed currently. Start the timout
            modifierlessTimeout_.start(600);
        } else {
            // A modifier is pressed. Stop the timeout
            modifierlessTimeout_.stop();
        }
    }

    void cancelRecording()
    {
        keySequence_ = oldKeySequence_;
        doneRecording();
    }

    // private slot
    void doneRecording();

    // members
    KeySequenceWidget *const q;
    QHBoxLayout *layout_ = nullptr;
    KeySequenceButton *keyButton_ = nullptr;
    QToolButton *clearButton_ = nullptr;
    QAction *keyCodeModeAction_ = nullptr;

    QList<Key> keySequence_;
    QList<Key> oldKeySequence_;
    QTimer modifierlessTimeout_;
    bool allowModifierless_ = false;
    KeyStates modifierKeys_;
    unsigned int qtModifierKeys_ = 0;
    bool isRecording_ = false;
    bool multiKeyShortcutsAllowed_ = false;
    bool allowModifierOnly_ = false;
    bool modifierAllowed_ = true;
    bool keycodeAllowed_ = true;
};

KeySequenceWidgetPrivate::KeySequenceWidgetPrivate(KeySequenceWidget *q)
    : q(q)
{
}

void KeySequenceWidgetPrivate::init()
{
    layout_ = new QHBoxLayout(q);
    layout_->setMargin(0);

    keyButton_ = new KeySequenceButton(this, q);
    keyButton_->setFocusPolicy(Qt::StrongFocus);
    keyButton_->setIcon(QIcon::fromTheme("configure"));
    layout_->addWidget(keyButton_);

    clearButton_ = new QToolButton(q);
    layout_->addWidget(clearButton_);

    keyCodeModeAction_ = new QAction(_("Key code mode"));
    keyCodeModeAction_->setCheckable(true);
    keyCodeModeAction_->setEnabled(isX11LikePlatform());
    q->setContextMenuPolicy(Qt::ActionsContextMenu);
    q->addAction(keyCodeModeAction_);

    if (qApp->isLeftToRight())
        clearButton_->setIcon(QIcon::fromTheme("edit-clear-locationbar-rtl"));
    else
        clearButton_->setIcon(QIcon::fromTheme("edit-clear-locationbar-ltr"));

    q->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

KeySequenceWidget::KeySequenceWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KeySequenceWidgetPrivate(this))
{
    d->init();
    setFocusProxy(d->keyButton_);
    connect(d->keyButton_, &QPushButton::clicked, this, &KeySequenceWidget::captureKeySequence);
    connect(d->clearButton_, &QPushButton::clicked, this, &KeySequenceWidget::clearKeySequence);
    connect(&d->modifierlessTimeout_, &QTimer::timeout, this, [this]() {
        d->doneRecording();
    });
    d->updateShortcutDisplay();
}

KeySequenceWidget::~KeySequenceWidget()
{
    delete d;
}

bool KeySequenceWidget::multiKeyShortcutsAllowed() const
{
    return d->multiKeyShortcutsAllowed_;
}

void KeySequenceWidget::setMultiKeyShortcutsAllowed(bool allowed)
{
    d->multiKeyShortcutsAllowed_ = allowed;
}

bool KeySequenceWidget::isModifierAllowed() const
{
    return d->modifierAllowed_;
}

void KeySequenceWidget::setModifierAllowed(bool allowed)
{
    d->modifierAllowed_ = allowed;
}

bool KeySequenceWidget::isKeycodeAllowed() const
{
    return d->keycodeAllowed_;
}

void KeySequenceWidget::setKeycodeAllowed(bool allowed)
{
    if (d->keycodeAllowed_ == allowed) {
        return;
    }
    d->keycodeAllowed_ = allowed;
    if (allowed) {
        d->keyCodeModeAction_->setChecked(false);
        addAction(d->keyCodeModeAction_);
    } else {
        removeAction(d->keyCodeModeAction_);
    }
}

void KeySequenceWidget::setModifierlessAllowed(bool allow)
{
    d->allowModifierless_ = allow;
}

bool KeySequenceWidget::isModifierlessAllowed() const
{
    return d->allowModifierless_;
}

bool KeySequenceWidget::isModifierOnlyAllowed() const
{
    return d->allowModifierOnly_;
}

void KeySequenceWidget::setModifierOnlyAllowed(bool allow)
{
    d->allowModifierOnly_ = allow;
}

void KeySequenceWidget::setClearButtonShown(bool show)
{
    d->clearButton_->setVisible(show);
}

bool KeySequenceWidget::isClearButtonVisible() const
{
    return d->clearButton_->isVisible();
}

// slot
void KeySequenceWidget::captureKeySequence()
{
    d->startRecording();
}

const QList<Key> &KeySequenceWidget::keySequence() const
{
    return d->keySequence_;
}

// slot
void KeySequenceWidget::setKeySequence(const QList<Key> &seq)
{
    // oldKeySequence holds the key sequence before recording started, if
    // setKeySequence()
    // is called while not recording then set oldKeySequence to the existing
    // sequence so
    // that the keySequenceChanged() signal is emitted if the new and previous
    // key
    // sequences are different
    if (!d->isRecording_) {
        d->oldKeySequence_ = d->keySequence_;
    }

    d->keySequence_ = QList<Key>();
    for (auto key : seq) {
        if (key.isValid()) {
            d->keySequence_ << key;
        }
    }
    d->doneRecording();
}

// slot
void KeySequenceWidget::clearKeySequence()
{
    setKeySequence(QList<Key>());
}

void KeySequenceWidgetPrivate::startRecording()
{
    modifierKeys_ = 0;
    oldKeySequence_ = keySequence_;
    keySequence_ = QList<Key>();
    isRecording_ = true;
    keyButton_->grabKeyboard();

    if (!QWidget::keyboardGrabber()) {
        qWarning() << "Failed to grab the keyboard! Most likely qt's nograb "
                      "option is active";
    }

    keyButton_->setDown(true);
    updateShortcutDisplay();
}

void KeySequenceWidgetPrivate::doneRecording()
{
    modifierlessTimeout_.stop();
    isRecording_ = false;
    keyButton_->releaseKeyboard();
    keyButton_->setDown(false);

    if (keySequence_ == oldKeySequence_ && !allowModifierOnly_) {
        // The sequence hasn't changed
        updateShortcutDisplay();
        return;
    }

    Q_EMIT q->keySequenceChanged(keySequence_);

    updateShortcutDisplay();
}

void KeySequenceWidgetPrivate::updateShortcutDisplay()
{
    QString s =
        QString::fromUtf8(Key::keyListToString(keySequence_, KeyStringFormat::Localized).c_str());
    s.replace('&', QLatin1String("&&"));

    if (isRecording_) {
        if (modifierKeys_) {
            if (!s.isEmpty())
                s.append(",");
            if (modifierKeys_ & KeyState::Super)
                s += "Super+";
            if (modifierKeys_ & KeyState::Ctrl)
                s += "Control+";
            if (modifierKeys_ & KeyState::Alt)
                s += "Alt+";
            if (modifierKeys_ & KeyState::Shift)
                s += "Shift+";
            if (modifierKeys_ & KeyState::Hyper)
                s += "Hyper+";

        } else if (keySequence_.size() == 0) {
            s = "...";
        }
        // make it clear that input is still going on
        s.append(" ...");
    }

    if (s.isEmpty()) {
        s = _("Empty");
    }

    s.prepend(' ');
    s.append(' ');
    keyButton_->setText(s);
}

KeySequenceButton::~KeySequenceButton() { }

// prevent Qt from special casing Tab and Backtab
bool KeySequenceButton::event(QEvent *e)
{
    if (d->isRecording_ && e->type() == QEvent::KeyPress) {
        keyPressEvent(static_cast<QKeyEvent *>(e));
        return true;
    }

    // The shortcut 'alt+c' ( or any other dialog local action shortcut )
    // ended the recording and triggered the action associated with the
    // action. In case of 'alt+c' ending the dialog.  It seems that those
    // ShortcutOverride events get sent even if grabKeyboard() is active.
    if (d->isRecording_ && e->type() == QEvent::ShortcutOverride) {
        e->accept();
        return true;
    }

    return QPushButton::event(e);
}

void KeySequenceButton::keyPressEvent(QKeyEvent *e)
{
    int keyQt = e->key();
    if (keyQt == -1) {
        // Qt sometimes returns garbage keycodes, I observed -1, if it doesn't
        // know a key. We cannot do anything useful with those (several keys
        // have -1, indistinguishable) and QKeySequence.toString() will also
        // yield a garbage string.
        QMessageBox::warning(this,
                             _("The key you just pressed is not supported by Qt."),
                             _("Unsupported Key"));
        return d->cancelRecording();
    }

    // Same as Key::normalize();
    unsigned int newQtModifiers = e->modifiers() & (Qt::META | Qt::ALT | Qt::CTRL | Qt::SHIFT);
    KeyStates newModifiers;
    if (isX11LikePlatform()) {
        newModifiers = KeyStates(e->nativeModifiers())
            & KeyStates{ KeyState::Ctrl_Alt_Shift, KeyState::Hyper, KeyState::Super };
        newModifiers |= Key::keySymToStates(static_cast<KeySym>(e->nativeVirtualKey()));
    } else {
        if (newQtModifiers & Qt::META) {
            newModifiers |= KeyState::Super;
        }
        if (newQtModifiers & Qt::ALT) {
            newModifiers |= KeyState::Alt;
        }
        if (newQtModifiers & Qt::CTRL) {
            newModifiers |= KeyState::Ctrl;
        }
        if (newQtModifiers & Qt::SHIFT) {
            newModifiers |= KeyState::Shift;
        }
    }

    // don't have the return or space key appear as first key of the sequence
    // when they
    // were pressed to start editing - catch and them and imitate their effect
    if (!d->isRecording_ && ((keyQt == Qt::Key_Return || keyQt == Qt::Key_Space))) {
        d->startRecording();
        d->modifierKeys_ = newModifiers;
        d->qtModifierKeys_ = newQtModifiers;
        d->updateShortcutDisplay();
        return;
    }

    // We get events even if recording isn't active.
    if (!d->isRecording_)
        return QPushButton::keyPressEvent(e);

    e->accept();
    d->modifierKeys_ = newModifiers;
    d->qtModifierKeys_ = newQtModifiers;

    switch (keyQt) {
    case Qt::Key_AltGr: // or else we get unicode salad
        return;
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Alt:
    case Qt::Key_Super_L:
    case Qt::Key_Super_R:
    case Qt::Key_Hyper_L:
    case Qt::Key_Hyper_R:
    case Qt::Key_Meta:
    case Qt::Key_Menu: // unused (yes, but why?)
        d->controlModifierlessTimout();
        d->updateShortcutDisplay();
        break;
    default:
        // We now have a valid key press.
        if (keyQt) {
            if ((keyQt == Qt::Key_Backtab) && d->modifierKeys_.test(KeyState::Shift)) {
                keyQt = Qt::Key_Tab | d->qtModifierKeys_;
            } else {
                keyQt |= d->qtModifierKeys_;
            }

            Key key;
            if (d->keyCodeModeAction_->isChecked()) {
                key = Key::fromKeyCode(e->nativeScanCode(), d->modifierKeys_);
            } else {
                if (isX11LikePlatform()) {
                    key = Key(static_cast<KeySym>(e->nativeVirtualKey()),
                              KeyStates(e->nativeModifiers()))
                              .normalize();
                } else {
                    if (!keyQtToFcitx(keyQt, e->text(), MS_Unknown, key)) {
                        qDebug() << "FcitxQtKeySequenceButton::keyPressEvent() "
                                    "Failed to "
                                    "convert Qt key to fcitx: "
                                 << e;
                    }
                }
            }

            // Check the first key.
            if (d->keySequence_.size() == 0) {
                if (!d->allowModifierless_ && key.states() == 0) {
                    return;
                }
                if (!d->modifierAllowed_ && key.states() != 0) {
                    return;
                }
            }

            if (key.isValid()) {
                d->keySequence_ << key;
            }

            if ((!d->multiKeyShortcutsAllowed_) || (d->keySequence_.size() >= 4)) {
                d->doneRecording();
                return;
            }
            d->controlModifierlessTimout();
            d->updateShortcutDisplay();
        }
    }
}

void KeySequenceButton::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == -1) {
        // ignore garbage, see keyPressEvent()
        return;
    }

    if (!d->isRecording_)
        return QPushButton::keyReleaseEvent(e);

    e->accept();

    if (!d->multiKeyShortcutsAllowed_ && d->allowModifierOnly_
        && (e->key() == Qt::Key_Shift || e->key() == Qt::Key_Control || e->key() == Qt::Key_Meta
            || e->key() == Qt::Key_Alt)) {
        auto side = MS_Unknown;

        if (isX11LikePlatform()) {

            if (e->nativeVirtualKey() == FcitxKey_Control_L
                || e->nativeVirtualKey() == FcitxKey_Alt_L
                || e->nativeVirtualKey() == FcitxKey_Shift_L
                || e->nativeVirtualKey() == FcitxKey_Super_L) {
                side = MS_Left;
            }
            if (e->nativeVirtualKey() == FcitxKey_Control_R
                || e->nativeVirtualKey() == FcitxKey_Alt_R
                || e->nativeVirtualKey() == FcitxKey_Shift_R
                || e->nativeVirtualKey() == FcitxKey_Super_R) {
                side = MS_Right;
            }
        }
        int keyQt = e->key() | d->qtModifierKeys_;
        Key key;
        if (keyQtToFcitx(keyQt, e->text(), side, key)) {
            if (d->keyCodeModeAction_->isChecked()) {
                key = Key::fromKeyCode(e->nativeScanCode(), key.states());
            }
            d->keySequence_ = QList<Key>({ key });
        }
        d->doneRecording();
        return;
    }

    unsigned int newQtModifiers = e->modifiers() & (Qt::META | Qt::ALT | Qt::CTRL | Qt::SHIFT);
    KeyStates newModifiers;
    if (isX11LikePlatform()) {
        newModifiers = KeyStates(e->nativeModifiers())
            & KeyStates{ KeyState::Ctrl_Alt_Shift, KeyState::Hyper, KeyState::Super };
        newModifiers &= ~Key::keySymToStates(static_cast<KeySym>(e->nativeVirtualKey()));
    } else {
        if (newQtModifiers & Qt::META) {
            newModifiers |= KeyState::Super;
        }
        if (newQtModifiers & Qt::ALT) {
            newModifiers |= KeyState::Alt;
        }
        if (newQtModifiers & Qt::CTRL) {
            newModifiers |= KeyState::Ctrl;
        }
        if (newQtModifiers & Qt::SHIFT) {
            newModifiers |= KeyState::Shift;
        }
    }

    // if a modifier that belongs to the shortcut was released...
    if ((newModifiers & d->modifierKeys_) < d->modifierKeys_) {
        d->modifierKeys_ = newModifiers;
        d->controlModifierlessTimout();
        d->updateShortcutDisplay();
    }
}

// static
bool KeySequenceWidgetPrivate::isOkWhenModifierless(int keyQt)
{
    // this whole function is a hack, but especially the first line of code
    if (QKeySequence(keyQt).toString().length() == 1)
        return false;

    switch (keyQt) {
    case Qt::Key_Return:
    case Qt::Key_Space:
    case Qt::Key_Tab:
    case Qt::Key_Backtab: // does this ever happen?
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        return false;
    default:
        return true;
    }
}

} // namespace widgets
} // namespace dcc_im_plugin

#include "keysequencewidget.moc"
