#include "InputState.h"

#include "Dim.h"

InputState::InputState(Dim *dim)
    : dim_(dim) {
    auto &ims = dim_->enabledIMs();
    current_im_ = ims[0];
}

const QString &InputState::currentIM() const {
    return current_im_;
}
