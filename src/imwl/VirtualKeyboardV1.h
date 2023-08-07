// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VIRTUALKEYBOARDV1_H
#define VIRTUALKEYBOARDV1_H

#include "common.h"

#include <QObject>

struct wl_client;
struct wl_display;
struct wl_resource;

class QWaylandSeat;

class VirtualKeyboardV1Private;

class VirtualKeyboardV1 : public QObject
{
    Q_OBJECT

    friend class VirtualKeyboardV1Private;

public:
    VirtualKeyboardV1(struct ::wl_resource *seat, QObject *paernt);
    ~VirtualKeyboardV1();

    INIT_FUNCS_DEF

private:
    std::unique_ptr<VirtualKeyboardV1Private> d;
    QWaylandSeat *seat_;
};

#endif // !VIRTUALKEYBOARDV1_H
