#ifndef WL_CLIENT_ZWPINPUTMETHODMANAGERV2_H
#define WL_CLIENT_ZWPINPUTMETHODMANAGERV2_H

#include "Type.h"

#include <memory>

namespace wl {
namespace client {

class Seat;
class ZwpInputMethodV2;

class ZwpInputMethodManagerV2 : public Type<zwp_input_method_manager_v2>
{
public:
    ZwpInputMethodManagerV2(zwp_input_method_manager_v2 *val);
    ~ZwpInputMethodManagerV2();

    std::shared_ptr<ZwpInputMethodV2> getInputMethod(const std::shared_ptr<Seat> &seat);
};

} // namespace client
}; // namespace wl

#endif // !WL_CLIENT_ZWPINPUTMETHODMANAGERV2_H
