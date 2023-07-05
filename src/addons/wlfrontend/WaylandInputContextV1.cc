#include "WaylandInputContextV1.h"

using namespace org::deepin::dim;

WaylandInputContextV1::WaylandInputContextV1(Dim *dim, QObject *parent)
    : InputContext(dim, parent)
{
}

void WaylandInputContextV1::activate(struct zwp_input_method_context_v1 *id)
{
    input_method_context_v1_ = id;
    focusIn();
}

void WaylandInputContextV1::deactivate(struct zwp_input_method_context_v1 *id)
{
    if (input_method_context_v1_ == id) {
        input_method_context_v1_ = nullptr;
        focusOut();
    }
}
