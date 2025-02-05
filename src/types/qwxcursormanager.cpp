// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxcursormanager.h"
#include "qwcursor.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_xcursor_manager.h>
}

QW_BEGIN_NAMESPACE

void QWXCursorManager::destroy()
{
    wlr_xcursor_manager_destroy(handle());
}

wlr_xcursor_manager *QWXCursorManager::handle() const
{
    return reinterpret_cast<wlr_xcursor_manager*>(const_cast<QWXCursorManager*>(this));
}

QWXCursorManager *QWXCursorManager::from(wlr_xcursor_manager *handle)
{
    return reinterpret_cast<QWXCursorManager*>(handle);
}

QWXCursorManager *QWXCursorManager::create(const char *name, uint32_t size)
{
    return from(wlr_xcursor_manager_create(name, size));
}

bool QWXCursorManager::load(float scale)
{
    return wlr_xcursor_manager_load(handle(), scale);
}

wlr_xcursor *QWXCursorManager::getXCursor(const char *name, float scale) const
{
    return wlr_xcursor_manager_get_xcursor(handle(), name, scale);
}

void QWXCursorManager::setCursor(const char *name, QWCursor *cursor)
{
    wlr_xcursor_manager_set_cursor_image(handle(), name, cursor->handle());
}

QW_END_NAMESPACE
