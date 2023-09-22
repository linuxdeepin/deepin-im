// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Compositor.h"

using namespace wl::server;

const struct wl_compositor_interface Compositor::impl = {
    ResourceCallbackWrapper<&Compositor::wl_compositor_create_surface>::func,
    ResourceCallbackWrapper<&Compositor::wl_compositor_create_region>::func,
};

Compositor::Compositor()
    : Type()
{
}

Compositor::~Compositor() = default;
