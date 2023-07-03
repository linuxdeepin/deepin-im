#ifndef WAYLANDINPUTCONTEXTV1
#define WAYLANDINPUTCONTEXTV1

#include <dimcore/InputContext.h>

class WaylandInputContextV1 : public org::deepin::dim::InputContext
{
public:
    WaylandInputContextV1(org::deepin::dim::Dim *dim, QObject *parent = nullptr);

    void activate(struct zwp_input_method_context_v1 *id);
    void deactivate(struct zwp_input_method_context_v1 *id);

private:
    struct zwp_input_method_context_v1 *input_method_context_v1_;
};

#endif // !WAYLANDINPUTCONTEXTV1
