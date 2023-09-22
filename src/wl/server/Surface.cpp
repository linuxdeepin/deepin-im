// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Surface.h"

using namespace wl::server;

const struct wl_surface_interface Surface::impl = {
    ResourceCallbackWrapper<&Surface::wl_surface_destroy>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_attach>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_damage>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_frame>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_set_opaque_region>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_set_input_region>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_commit>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_set_buffer_transform>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_set_buffer_scale>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_damage_buffer>::func,
    ResourceCallbackWrapper<&Surface::wl_surface_offset>::func,
};

Surface::Surface()
    : Type()
{
}

Surface::~Surface() = default;
