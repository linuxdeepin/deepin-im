#ifndef DIM_H
#define DIM_H

#include <QObject>
#include <QDBusObjectPath>

class Dim : public QObject {
    Q_OBJECT

public:
    Dim(QObject *parent = nullptr);
    ~Dim() = default;

public:
    QDBusObjectPath CreateInputContext();
};

#endif // !DIM_H
