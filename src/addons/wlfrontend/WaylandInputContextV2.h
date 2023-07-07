#ifndef WAYLANDINPUTCONTEXTV1
#define WAYLANDINPUTCONTEXTV1

#include <dimcore/InputContext.h>

class WaylandInputContextV2 : public org::deepin::dim::InputContext
{
public:
    WaylandInputContextV2(org::deepin::dim::Dim *dim, QObject *parent = nullptr);

    void activate(struct zwp_input_method_v2 *zwp_input_method_v2);
    void deactivate(struct zwp_input_method_v2 *zwp_input_method_v2);
    void surroundingText(struct zwp_input_method_v2 *zwp_input_method_v2,
                         const char *text,
                         uint32_t cursor,
                         uint32_t anchor);
    void textChangeCause(struct zwp_input_method_v2 *zwp_input_method_v2, uint32_t cause);
    void contentType(struct zwp_input_method_v2 *zwp_input_method_v2,
                     uint32_t hint,
                     uint32_t purpose);
    void done(struct zwp_input_method_v2 *zwp_input_method_v2);
    void unavailable(struct zwp_input_method_v2 *zwp_input_method_v2);

private:
    struct zwp_input_method_context_v1 *input_method_context_v1_;
};

#endif // !WAYLANDINPUTCONTEXTV1
