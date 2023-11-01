// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <string>

namespace utilFuncs {

std::string translate(const std::string &s);
const char *translate(const char *s);
std::string translateCtx(const char *ctx, const std::string &s);
const char *translateCtx(const char *ctx, const char *s);

std::string translateDomain(const char *domain, const std::string &s);
const char *translateDomain(const char *domain, const char *s);
std::string translateDomainCtx(const char *domain, const char *ctx, const std::string &s);

const char *translateDomainCtx(const char *domain, const char *ctx, const char *s);
void registerDomain(const char *domain, const char *dir);
} // namespace utilFuncs

#ifndef IM_NO_TRANSLATE_MACRO

#  ifdef IM_GETTEXT_DOMAIN
#    define _(x) ::utilFuncs::translateDomain(FCITX_GETTEXT_DOMAIN, x)
#    define C_(c, x) ::utilFuncs::translateDomainCtx(FCITX_GETTEXT_DOMAIN, c, x)
#  else
#    define _(x) ::utilFuncs::translate(x)
#    define C_(c, x) ::utilFuncs::translateCtx(c, x)
#  endif

#  define D_(d, x) ::fcitx::translateDomain(d, x)

#  define NC_(c, x) (x)
#  define N_(x) (x)

#endif

#endif // _FCITX_UTILS_I18N_H_
