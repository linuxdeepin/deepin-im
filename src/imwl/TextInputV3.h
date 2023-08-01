#ifndef TEXTINPUTV3_H
#define TEXTINPUTV3_H

#include "common.h"

#include <QObject>

struct wl_client;
struct wl_display;
struct wl_resource;

class Core;
class TextInputV3Private;

class TextInputV3 : public QObject
{
    Q_OBJECT

    friend class TextInputV3Private;

public:
    TextInputV3(Core *core, struct ::wl_resource *seat, QObject *parent);
    ~TextInputV3();

    INIT_FUNCS_DEF

    void sendPreeditString(const QString &text, int32_t cursor_begin, int32_t cursor_end);
    void sendCommitString(const QString &text);
    void sendDone(uint32_t serial);

private:
    Core *m_core;
    struct ::wl_resource *m_seat;
    std::unique_ptr<TextInputV3Private> d;
};

#endif // !TEXTINPUTV3_H
