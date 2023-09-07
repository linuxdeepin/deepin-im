// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ProxyAddon.h"

using namespace org::deepin::dim;

ProxyAddon::ProxyAddon(Dim *dim, const QString &key, const QString &iconName)
    : InputMethodAddon(dim, key, iconName)
{
}

ProxyAddon::~ProxyAddon() { }
