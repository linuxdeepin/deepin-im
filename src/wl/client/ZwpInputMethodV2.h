#ifndef WL_ZWPINPUTMETHODV2_H
#define WL_ZWPINPUTMETHODV2_H

#include "wl/Type.h"

#include <memory>

namespace wl {

class ZwpInputMethodKeyboardGrabV2;

class ZwpInputMethodV2 : public Type<zwp_input_method_v2>
{
public:
    ZwpInputMethodV2(zwp_input_method_v2 *val);
    ~ZwpInputMethodV2();

    std::shared_ptr<ZwpInputMethodKeyboardGrabV2> grabKeyboard();

    void commitString(const char *text);
    void setPreeditString(const char *text, int32_t cursorBegin, int32_t cursorEnd);
    void commit(uint32_t serial);
};

} // namespace wl

#endif // !WL_ZWPINPUTMETHODV2_H
