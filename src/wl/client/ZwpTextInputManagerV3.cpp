#include "ZwpTextInputManagerV3.h"

#include "Seat.h"
#include "ZwpTextInputV3.h"

using namespace wl::client;

ZwpTextInputManagerV3::ZwpTextInputManagerV3(zwp_text_input_manager_v3 *val)
    : Type(val)
{
}

ZwpTextInputManagerV3::~ZwpTextInputManagerV3()
{
    zwp_text_input_manager_v3_destroy(get());
}

std::shared_ptr<ZwpTextInputV3>
ZwpTextInputManagerV3::getTextInput(const std::shared_ptr<Seat> &seat)
{
    return std::make_shared<ZwpTextInputV3>(
        zwp_text_input_manager_v3_get_text_input(get(), seat->get()));
}
