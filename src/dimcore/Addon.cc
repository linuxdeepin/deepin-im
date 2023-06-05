#include "Addon.h"

using namespace org::deepin::dim;

Addon::Addon(Dim *dim, const QString &key)
    : dim_(dim)
    , key_(key) {
}

Addon::~Addon() {
}

Dim *Addon::dim() const {
    return dim_;
}

const QString &Addon::key() const {
    return key_;
}
