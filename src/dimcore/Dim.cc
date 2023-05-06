#include "Dim.h"

#include "InputContext.h"

Dim::Dim(QObject *parent)
    : QObject(parent) {
}

Dim::~Dim() {
}

uint32_t Dim::newInputContext() {
    auto *ctx = new InputContext(this);
    inputContexts_.insert(ctx->id(), ctx);

    connect(ctx, &InputContext::destroyed, this, [this, ctx]() { inputContexts_.remove(ctx->id()); });

    return ctx->id();
}
