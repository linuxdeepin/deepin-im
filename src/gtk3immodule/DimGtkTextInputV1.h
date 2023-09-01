// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DIM_TEXT_INPUT_V1_H
#define DIM_TEXT_INPUT_V1_H

#include "imcontext.h"
#include "wl/client/ZwpDimTextInputV1.h"

class DimGtkTextInputV1 : public wl::client::ZwpDimTextInputV1
{
public:
    DimGtkTextInputV1(struct ::zwp_dim_text_input_v1 *text_input, DimIMContextWaylandGlobal *global);
    ~DimGtkTextInputV1() override;

    inline void text_input_preedit(const char *text, int32_t cursor_begin, int32_t cursor_end)
    {
        return zwp_dim_text_input_v1_preedit_string(text, cursor_begin, cursor_end);
    }

protected:
    void zwp_dim_text_input_v1_enter() override;
    void zwp_dim_text_input_v1_leave() override;
    void zwp_dim_text_input_v1_modifiers_map(struct wl_array *map) override;
    void zwp_dim_text_input_v1_preedit_string(const char *text,
                                              int32_t cursor_begin,
                                              int32_t cursor_end) override;
    void zwp_dim_text_input_v1_commit_string(const char *text) override;
    void zwp_dim_text_input_v1_delete_surrounding_text(uint32_t before_length,
                                                       uint32_t after_length) override;
    void zwp_dim_text_input_v1_done(uint32_t serial) override;
    void zwp_dim_text_input_v1_keysym(
        uint32_t serial, uint32_t time, uint32_t sym, uint32_t state, uint32_t modifiers) override;

private:
    DimIMContextWaylandGlobal *global_;
};

#endif // DIM_TEXT_INPUT_V1_H
