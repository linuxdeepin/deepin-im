#ifndef _ORG_DEEPIN_DIM_COMMON_H_
#define _ORG_DEEPIN_DIM_COMMON_H_

#include "message.h"

#include <QVariant>

namespace org {
namespace deepin {
namespace dim {

enum { BATCHED_COMMIT_STRING = 0, BATCHED_PREEDIT, BATCHED_FORWARD_KEY };

using BatchEvent = dbus::DBusStruct<uint32_t, QVariant>;
using DBusForwardKey = dbus::DBusStruct<uint32_t, uint32_t, bool>;
using PreeditKey = dbus::DBusStruct<std::vector<dbus::DBusStruct<QString, int32_t>>, int32_t>;

} // namespace dim
} // namespace deepin
} // namespace org

Q_DECLARE_METATYPE(org::deepin::dim::BatchEvent)
Q_DECLARE_METATYPE(org::deepin::dim::DBusForwardKey)
Q_DECLARE_METATYPE(org::deepin::dim::PreeditKey)


#endif // _ORG_DEEPIN_DIM_COMMON_H_