#ifndef WL_ZWPINPUTMETHODMANAGERV2_H
#define WL_ZWPINPUTMETHODMANAGERV2_H

#include "Type.h"

#include <memory>

namespace wl {

class Seat;
class ZwpInputMethodV2;

class ZwpInputMethodManagerV2 : public Type<zwp_input_method_manager_v2>
{
public:
    ZwpInputMethodManagerV2(zwp_input_method_manager_v2 *val);
    ~ZwpInputMethodManagerV2();

    std::shared_ptr<ZwpInputMethodV2> getInputMethod(const std::shared_ptr<Seat> &seat);
};

}; // namespace wl

#endif // !WL_ZWPINPUTMETHODMANAGERV2_H
