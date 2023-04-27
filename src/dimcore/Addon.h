#ifndef ADDON_H
#define ADDON_H

class Dim;

class Addon {
public:
    Addon(Dim *dim);
    ~Addon();

    Dim *dim();

private:
    Dim *dim_;
};

#define DIM_ADDON_FACTORY(name)                                                                                        \
    extern "C" {                                                                                                       \
    Addon *create(Dim *dim) { return new name(dim); }                                                                  \
    }

#endif // !ADDON_H
