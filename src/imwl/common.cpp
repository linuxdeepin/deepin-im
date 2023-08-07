// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "common.h"


uint32_t nextSerial() {
    static uint32_t current = 1;

    return current++;
}
