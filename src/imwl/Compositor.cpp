// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Compositor.h"

#include "DimTextInputManagerV1.h"
#include "DimTextInputV1.h"
#include "InputMethodManagerV2.h"
#include "InputMethodV2.h"
#include "VirtualKeyboardManagerV1.h"
#include "VirtualKeyboardV1.h"
#include "X11ActiveWindowMonitor.h"

#include <QAbstractEventDispatcher>
#include <QDebug>
#include <QSocketNotifier>
#include <QThread>
#include <QtWaylandCompositor/QWaylandSeat>

Compositor::Compositor()
    : activeWindowMonitor_(std::make_unique<X11ActiveWindowMonitor>())
{
    QObject::connect(activeWindowMonitor_.get(),
                     &X11ActiveWindowMonitor::activeWindowChanged,
                     [this]() {
                         activeWindowChanged(activeWindowMonitor_->activeWindowPid());
                     });
}

Compositor::~Compositor() { }

void Compositor::create()
{
    QWaylandCompositor::create();

    dimTextInputManagerV1_ = std::make_unique<DimTextInputManagerV1>();
    dimTextInputManagerV1_->init(display());

    // textInputManagerV3_ = std::make_unique<TextInputManagerV3>();
    // textInputManagerV3_->init(display());

    inputMethodManagerV2_ = std::make_unique<InputMethodManagerV2>();
    inputMethodManagerV2_->init(display());

    virtualKeyboardManagerV1_ = std::make_unique<VirtualKeyboardManagerV1>();
    virtualKeyboardManagerV1_->init(display());

    dimTextInputV1_ = std::make_shared<DimTextInputV1>(defaultSeat());
    // textInputV3_ = std::make_shared<TextInputV3>(defaultSeat());
    inputMethodV2_ = std::make_shared<InputMethodV2>(defaultSeat());
    virtualKeyboardV1_ = std::make_shared<VirtualKeyboardV1>(defaultSeat());

    activeWindowChanged(activeWindowMonitor_->activeWindowPid());
}

void Compositor::activeWindowChanged(pid_t pid)
{
    qDebug() << "active window changed, pid:" << pid;
    dimTextInputV1_->leavePid(activePid_);
    dimTextInputV1_->enterPid(pid);
    activePid_ = pid;
}
