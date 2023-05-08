#ifndef ADDON_H
#define ADDON_H

class Dim;

class Addon {
public:
    explicit Addon(Dim *dim);
    ~Addon();

    Dim *dim();

private:
    Dim *dim_;
};

#define DIM_ADDON_FACTORY(name)                                                                                        \
    extern "C" {                                                                                                       \
    Addon *create(Dim *dim) { return new name(dim); }                                                                  \
    }

typedef Addon *addonCreate(Dim *);

#endif // !ADDON_H
