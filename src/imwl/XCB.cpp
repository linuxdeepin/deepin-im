// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "XCB.h"

#include <QDebug>
#include <QSocketNotifier>

XCB::XCB()
    : QObject()
{
    xconn_.reset(xcb_connect(nullptr, &defaultScreenNbr_));

    if (int err = xcb_connection_has_error(xconn_.get())) {
        qWarning() << "xcb connect failed";
        return;
    }

    setup_ = xcb_get_setup(xconn_.get());
    screen_ = screenOfDisplay(defaultScreenNbr_);

    xcbFd_ = xcb_get_file_descriptor(xconn_.get());
    socketNotifier_ = new QSocketNotifier(xcbFd_, QSocketNotifier::Type::Read, this);
    connect(socketNotifier_, &QSocketNotifier::activated, this, &XCB::onXCBEvent);
}

XCB::~XCB() { }

xcb_atom_t XCB::getAtom(const char *atomName)
{
    auto reply = XCB_REPLY(xcb_intern_atom, xconn_.get(), 0, strlen(atomName), atomName);
    if (!reply) {
        throw std::runtime_error("xcb_intern_atom failed");
        return 0;
    }

    return reply->atom;
}

std::vector<char> XCB::getProperty(xcb_window_t window, xcb_atom_t property, uint32_t size) {
    std::vector<char> buff(size);
    getPropertyAux(buff, window, property, 0, size);

    return buff;
}

std::vector<char> XCB::getProperty(xcb_window_t window, xcb_atom_t property)
{
    // Don't read anything, just get the size of the property data
    auto reply = XCB_REPLY(xcb_get_property,
                           xconn_.get(),
                           false,
                           window,
                           property,
                           XCB_GET_PROPERTY_TYPE_ANY,
                           0,
                           0);
    if (!reply || reply->type == XCB_NONE) {
        qWarning("no reply");
        return {};
    }

    uint32_t offset = 0;
    auto bytesLeft = reply->bytes_after;
    qInfo() << "size: " << bytesLeft;
    std::vector<char> buff(bytesLeft);

    while (bytesLeft > 0) {
        std::tie(offset, bytesLeft) =
            getPropertyAux(buff, window, property, offset, UINT_MAX);
    }

    return buff;
}

xcb_screen_t *XCB::screenOfDisplay(int screen)
{
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup_);
    for (; iter.rem; --screen, xcb_screen_next(&iter)) {
        if (screen == 0) {
            return iter.data;
        }
    }

    return nullptr;
}

void XCB::onXCBEvent(QSocketDescriptor socket, QSocketNotifier::Type activationEvent)
{
    std::unique_ptr<xcb_generic_event_t> event;
    while (event.reset(xcb_poll_for_event(xconn_.get())), event) {
        xcbEvent(event);
    }
}

std::tuple<uint32_t, uint32_t> XCB::getPropertyAux(std::vector<char> &buff,
                                                   xcb_window_t window,
                                                   xcb_atom_t property,
                                                   uint32_t offset,
                                                   uint32_t size)
{
    auto xcbOffset = offset * 4;

    auto reply = XCB_REPLY(xcb_get_property,
                           xconn_.get(),
                           false,
                           window,
                           property,
                           XCB_GET_PROPERTY_TYPE_ANY,
                           xcbOffset,
                           size / 4);
    if (!reply || reply->type == XCB_NONE) {
        qWarning("no reply");
        return { 0, 0 };
    }

    auto bytesLeft = reply->bytes_after;
    char *data = static_cast<char *>(xcb_get_property_value(reply.get()));
    int length = xcb_get_property_value_length(reply.get());

    if ((offset + length) > buff.size()) {
        qWarning("buffer overflow");
        length = buff.size() - offset;
        // escape loop
        bytesLeft = 0;
    }

    memcpy(buff.data() + offset, data, length);
    offset += length;

    return { offset, bytesLeft };
}
