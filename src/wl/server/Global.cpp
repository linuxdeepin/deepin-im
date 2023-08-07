// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Global.h"

using namespace wl::server;

Global::Global(const std::shared_ptr<Server> &server,
               const struct wl_interface *interface,
               const std::function<void(struct wl_client *client, uint32_t version, uint32_t id)>
                   &onBindCallback)
    : global_(wl_global_create(server->display(),
                               interface,
                               interface->version,
                               this,
                               GlobalCallbackWrapper<&Global::onBind>::func))
    , onBindCallback_(onBindCallback)
{
}

void Global::onBind([[maybe_unused]] struct wl_client *client,
                    [[maybe_unused]] uint32_t version,
                    [[maybe_unused]] uint32_t id)
{
    onBindCallback_(client, version, id);
}
