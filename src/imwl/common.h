#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

uint32_t nextSerial();

template<auto Func>
class Deleter
{
public:
    template<typename T>
    void operator()(T *ptr) const
    {
        if (ptr) {
            Func(ptr);
        }
    }
};

#define INIT_FUNCS_DEF                                \
  void init(struct ::wl_client *client, uint32_t id); \
  void init(struct ::wl_display *display);            \
  void init(struct ::wl_resource *resource);          \
                                                      \
  void add(struct ::wl_client *client);               \
  void add(struct ::wl_client *client, uint32_t id);

#define INIT_FUNCS(CLASSNAME)                                    \
  void CLASSNAME::init(struct ::wl_client *client, uint32_t id)  \
  {                                                              \
    d->init(client, id, CLASSNAME##Private::interfaceVersion()); \
  }                                                              \
                                                                 \
  void CLASSNAME::init(struct ::wl_display *display)             \
  {                                                              \
    d->init(display, CLASSNAME##Private::interfaceVersion());    \
  }                                                              \
                                                                 \
  void CLASSNAME::init(struct ::wl_resource *resource)           \
  {                                                              \
    d->init(resource);                                           \
  }                                                              \
                                                                 \
  void CLASSNAME::add(struct ::wl_client *client)                \
  {                                                              \
    d->add(client, CLASSNAME##Private::interfaceVersion());      \
  }                                                              \
                                                                 \
  void CLASSNAME::add(struct ::wl_client *client, uint32_t id)   \
  {                                                              \
    d->add(client, id, CLASSNAME##Private::interfaceVersion());  \
  }

#endif // !COMMON_H
