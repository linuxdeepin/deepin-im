#ifndef DIM_H
#define DIM_H

#include <QObject>

class Dim : public QObject {
public:
    Dim(QObject *parent = nullptr);
    ~Dim();
};

#endif // !DIM_H
