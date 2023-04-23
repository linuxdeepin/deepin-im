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

#endif // !ADDON_H
