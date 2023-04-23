#include "Addon.h"

Addon::Addon(Dim *dim)
    : dim_(dim) {
}

Addon::~Addon() {
}

Dim *Addon::dim() {
    return dim_;
}
