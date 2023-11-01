// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef KEYSEQUENCEWIDGET_H
#define KEYSEQUENCEWIDGET_H

#include "utils/key.h"

#include <QList>
#include <QPushButton>

using namespace dcc_im_plugin::utils;

namespace dcc_im_plugin {
namespace widgets {

enum ModifierSide { MS_Unknown = 0, MS_Left = 1, MS_Right = 2 };

class KeySequenceWidgetPrivate;

class KeySequenceWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool multiKeyShortcutsAllowed READ multiKeyShortcutsAllowed WRITE
                   setMultiKeyShortcutsAllowed)

    Q_PROPERTY(bool modifierlessAllowed READ isModifierlessAllowed WRITE
                   setModifierlessAllowed)

    Q_PROPERTY(
        bool modifierAllowed READ isModifierAllowed WRITE setModifierAllowed)

    Q_PROPERTY(
        bool keycodeAllowed READ isKeycodeAllowed WRITE setKeycodeAllowed)

    Q_PROPERTY(bool modifierOnlyAllowed READ isModifierOnlyAllowed WRITE
                   setModifierOnlyAllowed)

public:
    /**
     * Constructor.
     */
    explicit KeySequenceWidget(QWidget *parent = 0);

    /**
     * Destructs the widget.
     */
    virtual ~KeySequenceWidget();

    /**
     * @brief Set whether allow multiple shortcuts.
     *
     * @param  allow
     */
    void setMultiKeyShortcutsAllowed(bool allow);
    bool multiKeyShortcutsAllowed() const;

    /**
     * @brief Set whether allow modifier less that produce text, such as just
     * key A.
     *
     * @param allow
     */
    void setModifierlessAllowed(bool allow);
    bool isModifierlessAllowed() const;

    /**
     * @brief Set whether allow key that has modifier.
     *
     * @param allow
     * @since 5.0.12
     */
    void setModifierAllowed(bool allow);
    bool isModifierAllowed() const;

    /**
     * @brief Set whether allow key to use key code.
     *
     * @param allow
     * @since 5.0.12
     */
    void setKeycodeAllowed(bool allow);
    bool isKeycodeAllowed() const;

    /**
     * @brief Set whether allow modifier only key, such as only left control.
     *
     * @param allow allow modifier only key to be captured.
     */
    void setModifierOnlyAllowed(bool allow);
    bool isModifierOnlyAllowed() const;

    void setClearButtonShown(bool show);
    bool isClearButtonVisible() const;

    const QList<Key> &keySequence() const;

Q_SIGNALS:
    void keySequenceChanged(const QList<Key> &seq);

public Q_SLOTS:
    void captureKeySequence();
    void setKeySequence(const QList<Key> &seq);
    void clearKeySequence();

private:
    friend class KeySequenceWidgetPrivate;
    KeySequenceWidgetPrivate *const d;

    Q_DISABLE_COPY(KeySequenceWidget)
};
} // namespace widgets
} // namespace dcc_im_plugin

#endif // _WIDGETSADDONS_FCITXQTKEYSEQUENCEWIDGET_H_
