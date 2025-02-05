// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;
struct wlr_compositor;
struct wlr_surface;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWRenderer;
class QWCompositorPrivate;
class QW_EXPORT QWCompositor : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWCompositor)
public:
    inline wlr_compositor *handle() const {
        return QWObject::handle<wlr_compositor>();
    }

    static QWCompositor *get(wlr_compositor *handle);
    static QWCompositor *from(wlr_compositor *handle);
    static QWCompositor *create(QWDisplay *display, QWRenderer *renderer);

Q_SIGNALS:
    void beforeDestroy(QWCompositor *self);
    // TODO: make to QWSurface
    void newSurface(wlr_surface *surface);

private:
    QWCompositor(wlr_compositor *handle, bool isOwner);
    ~QWCompositor() = default;
};

QW_END_NAMESPACE
