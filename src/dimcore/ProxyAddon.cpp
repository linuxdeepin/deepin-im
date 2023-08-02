// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ProxyAddon.h"

using namespace org::deepin::dim;

ProxyAddon::ProxyAddon(Dim *dim, const QString &key)
    : InputMethodAddon(dim, key)
{
}

ProxyAddon::~ProxyAddon() { }
