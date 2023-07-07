#ifndef WAYLANDINPUTCONTEXTV1
#define WAYLANDINPUTCONTEXTV1

#include "WlType.h"
#include "wayland-input-method-unstable-v2-client-protocol.h"

#include <dimcore/InputContext.h>

class WaylandInputContextV2
{
public:
    WaylandInputContextV2(const std::shared_ptr<WlType<zwp_input_method_v2>> &im);

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

    // virtual void updatePreedit(const org::deepin::dim::PreeditKey &key) override;
    // virtual void updateCommitString(const QString &text) override;
    // virtual void forwardKey(const org::deepin::dim::ForwardKey &key) override;

private:
    std::shared_ptr<WlType<zwp_input_method_v2>> im_;

    int id() { return 0; }
};

#endif // !WAYLANDINPUTCONTEXTV1
