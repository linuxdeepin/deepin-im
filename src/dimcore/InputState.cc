#include "InputState.h"

#include "Dim.h"

using namespace org::deepin::dim;

InputState::InputState(Dim *dim)
    : dim_(dim)
{
    auto &ims = dim_->enabledIMs();
    current_im_ = ims[0];
}

const QString &InputState::currentIM() const
{
    return current_im_;
}
