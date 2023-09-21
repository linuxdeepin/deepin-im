/*
 * SPDX-FileCopyrightText: 2017~2017 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _DIM_KEY_H_
#define _DIM_KEY_H_

#include <QString>
#include <cstdint>

int keySymToQtKey(uint32_t keysym, const QString &text);

#endif // _DIM_KEY_H_
