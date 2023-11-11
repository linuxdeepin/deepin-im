#include "XcbHelper.h"

#include <memory>
#include <stdexcept>

#include <limits.h>
#include <string.h>

XcbHelper::XcbHelper() { }

XcbHelper::~XcbHelper() = default;

void XcbHelper::flush()
{
    xcb_flush(conn_);
}

xcb_atom_t XcbHelper::getAtom(const std::string &atomName)
{
    auto iter = atoms_.find(atomName);
    if (iter != atoms_.end()) {
        return iter->second;
    }

    auto reply = XCB_REPLY(xcb_intern_atom, conn_, 0, atomName.length(), atomName.c_str());
    if (!reply) {
        throw std::runtime_error("xcb_intern_atom failed");
        return 0;
    }

    auto atom = reply->atom;
    atoms_.emplace(atomName, atom);
    return atom;
}

std::vector<char> XcbHelper::getProperty(xcb_window_t window,
                                         const std::string &property,
                                         uint32_t size)
{
    std::vector<char> buff(size);

    uint32_t offset = 0;
    auto bytesLeft = size;
    std::tie(offset, bytesLeft) = getPropertyAux(buff, window, property, 0, size);
    if (offset == 0 && bytesLeft == 0) {
        return {};
    }

    return buff;
}

std::vector<char> XcbHelper::getProperty(xcb_window_t window, const std::string &property)
{
    // Don't read anything, just get the size of the property data
    auto reply = XCB_REPLY(xcb_get_property,
                           conn_,
                           false,
                           window,
                           getAtom(property),
                           XCB_GET_PROPERTY_TYPE_ANY,
                           0,
                           0);
    if (!reply || reply->type == XCB_NONE) {
        // qWarning() << "no reply:" << windowToString(window) << property.c_str();
        return {};
    }

    uint32_t offset = 0;
    auto bytesLeft = reply->bytes_after;
    std::vector<char> buff(bytesLeft);

    while (bytesLeft > 0) {
        std::tie(offset, bytesLeft) = getPropertyAux(buff, window, property, offset, UINT_MAX);
    }

    return buff;
}

std::tuple<uint32_t, uint32_t> XcbHelper::getPropertyAux(std::vector<char> &buff,
                                                         xcb_window_t window,
                                                         const std::string &property,
                                                         uint32_t offset,
                                                         uint32_t size)
{
    auto xcbOffset = offset * 4;

    auto reply = XCB_REPLY(xcb_get_property,
                           conn_,
                           false,
                           window,
                           getAtom(property),
                           XCB_GET_PROPERTY_TYPE_ANY,
                           xcbOffset,
                           size / 4);
    if (!reply || reply->type == XCB_NONE) {
        // qWarning() << "no reply:" << windowToString(window) << property.c_str();
        return { 0, 0 };
    }

    auto bytesLeft = reply->bytes_after;
    char *data = static_cast<char *>(xcb_get_property_value(reply.get()));
    int length = xcb_get_property_value_length(reply.get());

    if ((offset + length) > buff.size()) {
        // qWarning("buffer overflow");
        length = buff.size() - offset;
        // escape loop
        bytesLeft = 0;
    }

    memcpy(buff.data() + offset, data, length);
    offset += length;

    return { offset, bytesLeft };
}
