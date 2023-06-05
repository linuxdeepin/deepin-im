#ifndef ADDON_H
#define ADDON_H

#include <QObject>

namespace org {
namespace deepin {
namespace dim {

class Dim;

class Addon : public QObject
{
    Q_OBJECT

public:
    explicit Addon(Dim *dim, const QString &key);
    virtual ~Addon();

    Dim *dim() const;
    const QString &key() const;

private:
    Dim *dim_;
    QString key_;
};

#define DIM_ADDON_FACTORY(name) \
    extern "C" {                \
    Addon *create(Dim *dim)     \
    {                           \
        return new name(dim);   \
    }                           \
    }

typedef Addon *addonCreate(Dim *);

} // namespace dim
} // namespace deepin
} // namespace org

#endif // !ADDON_H
