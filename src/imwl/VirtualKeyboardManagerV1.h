// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALKEYBOARDMANAGERV1_H
#define VIRTUALKEYBOARDMANAGERV1_H

#include "common.h"

#include <QObject>

struct wl_client;
struct wl_display;
struct wl_resource;

class VirtualKeyboardManagerV1Private;
class VirtualKeyboardV1;

class VirtualKeyboardManagerV1 : public QObject
{
    Q_OBJECT

    friend class VirtualKeyboardManagerV1Private;

public:
    VirtualKeyboardManagerV1();
    ~VirtualKeyboardManagerV1();

    INIT_FUNCS_DEF

private:
    std::unique_ptr<VirtualKeyboardManagerV1Private> d;
    std::unordered_map<struct ::wl_resource * /* seat */, VirtualKeyboardV1 *> virtualKeyboards_;
};

#endif // !VIRTUALKEYBOARDMANAGERV1_H
