#ifndef WL_CLIENT_ZWPTEXTINPUTMANAGERV3_H
#define WL_CLIENT_ZWPTEXTINPUTMANAGERV3_H

#include "Type.h"
#include "wayland-text-input-unstable-v3-client-protocol.h"

#include <memory>

namespace wl {
namespace client {

class ZwpTextInputV3;
class Seat;

class ZwpTextInputManagerV3 : public Type<zwp_text_input_manager_v3>
{
public:
    ZwpTextInputManagerV3(zwp_text_input_manager_v3 *val);
    ~ZwpTextInputManagerV3();

    std::shared_ptr<ZwpTextInputV3> getTextInput(const std::shared_ptr<Seat> &seat);

private:
};

} // namespace client
} // namespace wl

#endif // !WL_CLIENT_ZWPTEXTINPUTMANAGERV3_H
