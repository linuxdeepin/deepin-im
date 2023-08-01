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
