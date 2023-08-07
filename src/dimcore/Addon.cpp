// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Addon.h"

using namespace org::deepin::dim;

Addon::Addon(Dim *dim, const QString &key)
    : dim_(dim)
    , key_(key)
{
}

Addon::~Addon() { }
