// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef IMWL_COMMON_H
#define IMWL_COMMON_H

#include <stdint.h>

uint32_t nextSerial();

#define INIT_FUNCS_DEF                     \
  void init(struct ::wl_display *display); \
                                           \
  void add(struct ::wl_client *client, uint32_t id);

#define INIT_FUNCS(CLASSNAME)                                  \
  void CLASSNAME::init(struct ::wl_display *display)           \
  {                                                            \
    d->init(display);                                          \
  }                                                            \
                                                               \
  void CLASSNAME::add(struct ::wl_client *client, uint32_t id) \
  {                                                            \
    d->add(client, id);                                        \
  }

#endif // !IMWL_COMMON_H
