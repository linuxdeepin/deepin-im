// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_H
#define CORE_H

class TextInputManagerV3;
class InputMethodManagerV2;

class Core
{
public:
    virtual TextInputManagerV3 *getTextInputManagerV3() = 0;
    virtual InputMethodManagerV2 *getInputMethodManagerV2() = 0;
};

#endif // !CORE_H
