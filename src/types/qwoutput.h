// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwoutputinterface.h>

#include <QObject>
#include <type_traits>

struct wlr_output;
struct wlr_output_mode;
struct wlr_output_state;
struct wlr_drm_format_set;
struct wlr_output_cursor;
struct wlr_surface;
struct wlr_buffer;
struct wl_resource;
struct wlr_output_event_present;
struct wlr_output_event_damage;
struct wlr_output_event_bind;
struct wlr_output_event_precommit;
struct wlr_output_event_commit;

struct pixman_region32;

QW_BEGIN_NAMESPACE

class QWAllocator;
class QWRenderer;
class QWBuffer;
class QWOutputInterface;
class QWOutputPrivate;
class QW_EXPORT QWOutput : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWOutput)
public:
    inline wlr_output *handle() const {
        return QWObject::handle<wlr_output>();
    }

    static QWOutput *get(wlr_output *handle);
    static QWOutput *from(wlr_output *handle);
    static QWOutput *from(wl_resource *resource);
    template<class Interface, typename... Args>
    inline static typename std::enable_if<std::is_base_of<QWOutputInterface, Interface>::value, QWOutput*>::type
    create(Args&&... args) {
        Interface *i = new Interface();
        i->QWOutputInterface::template init<Interface>(std::forward<Args>(args)...);
        return new QWOutput(i->handle(), true);
    }

    void enable(bool enable);
    void createGlobal();
    void destroyGlobal();
    bool initRender(QWAllocator *allocator, QWRenderer *renderer);
    wlr_output_mode *preferredMode() const;

    void setMode(wlr_output_mode *mode);
    void setCustomMode(const QSize &size, int32_t refresh);
    void setTransform(int wl_output_transform);
    void enableAdaptiveSync(bool enabled);
    void setRenderFormat(uint32_t format);
    void setScale(float scale);
    void setSubpixel(int wl_output_subpixel);
    void setName(const QByteArray &name);
    void setDescription(const QByteArray &desc);
    void scheduleDone();

    QSize transformedResolution() const;
    QSize effectiveResolution() const;

    bool attachRender(int *bufferAge);
    void lockAttachRender(bool lock);
    void attachBuffer(QWBuffer *buffer);

    uint32_t preferredReadFormat() const;
    void setDamage(pixman_region32 *damage);
    bool test();
    bool commit();
    void rollback();
    bool testState(wlr_output_state *state);
    bool commitState(wlr_output_state *state);
    void scheduleFrame();

    size_t getGammaSize() const;
    void setGamma(size_t size, const uint16_t *r, const uint16_t *g, const uint16_t *b);
    void lockSoftwareCursors(bool lock);
    void renderSoftwareCursors(pixman_region32 *damage);
    const wlr_drm_format_set *getPrimaryFormats(uint32_t bufferCaps);

Q_SIGNALS:
    void beforeDestroy(QWOutput *self);
    void frame();
    void damage(wlr_output_event_damage *event);
    void needsFrame();
    void precommit(wlr_output_event_precommit *event);
    void commit(wlr_output_event_commit *event);
    void present(wlr_output_event_present *event);
    void bind(wlr_output_event_bind *event);
    void descriptionChanged();

private:
    QWOutput(wlr_output *handle, bool isOwner);
    ~QWOutput() = default;
};

class QW_EXPORT QWOutputCursor
{
public:
    void destroy();
    wlr_output_cursor *handle() const;

    static QWOutputCursor *from(wlr_output_cursor *handle);
    static QWOutputCursor *create(QWOutput *output);

    bool setImage(const QImage &image, const QPoint &hotspot);
    void setSurface(wlr_surface *surface, const QPoint &hotspot);
    bool setBuffer(QWBuffer *buffer, const QPoint &hotspot);
    bool move(const QPointF &pos);

private:
    QWOutputCursor() = default;
    ~QWOutputCursor() = default;
};

QW_END_NAMESPACE
