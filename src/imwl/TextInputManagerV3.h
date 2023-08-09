// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TEXTINPUTMANAGERV3_H
#define TEXTINPUTMANAGERV3_H

#include "common.h"

#include <QObject>

struct wl_client;
struct wl_display;
struct wl_resource;

class Core;
class TextInputManagerV3Private;
class TextInputV3;

class TextInputManagerV3 : public QObject
{
    Q_OBJECT

    friend class TextInputManagerV3Private;

public:
    TextInputManagerV3(Core *core);
    ~TextInputManagerV3();

    INIT_FUNCS_DEF

    TextInputV3 *getTextInputV4BySeat(struct ::wl_resource *seat);

private:
    std::unique_ptr<TextInputManagerV3Private> d;
    Core *core_;
    std::unordered_map<struct ::wl_resource * /* seat */, TextInputV3 *> textInputs_;
};

#endif // !TEXTINPUTMANAGERV3_H
