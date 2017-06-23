/* $Id: vbox_drv.h 67175 2017-05-31 14:38:02Z vboxsync $ */
/** @file
 * VirtualBox Additions Linux kernel video driver
 */

/*
 * Copyright (C) 2013-2017 Oracle Corporation
 * This file is based on ast_drv.h
 * Copyright 2012 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * Authors: Dave Airlie <airlied@redhat.com>
 *          Michael Thayer <michael.thayer@oracle.com,
 *          Hans de Goede <hdegoede@redhat.com>
 */
#ifndef __VBOX_DRV_H__
#define __VBOX_DRV_H__

#define LOG_GROUP LOG_GROUP_DEV_VGA

#include <VBoxVideoGuest.h>

#include <drm/drmP.h>
#include <drm/drm_fb_helper.h>

#include <drm/ttm/ttm_bo_api.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_memory.h>
#include <drm/ttm/ttm_module.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
# include <drm/drm_gem.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
# include <drm/drm_encoder.h>
#endif

/* #include "vboxvideo.h" */

#include "product-generated.h"

#define DRIVER_AUTHOR       VBOX_VENDOR

#define DRIVER_NAME         "vboxvideo"
#define DRIVER_DESC         VBOX_PRODUCT " Graphics Card"
#define DRIVER_DATE         "20130823"

#define DRIVER_MAJOR        1
#define DRIVER_MINOR        0
#define DRIVER_PATCHLEVEL   0

#define VBOX_MAX_CURSOR_WIDTH  64
#define VBOX_MAX_CURSOR_HEIGHT 64
#define CURSOR_PIXEL_COUNT VBOX_MAX_CURSOR_WIDTH * VBOX_MAX_CURSOR_HEIGHT
#define CURSOR_DATA_SIZE CURSOR_PIXEL_COUNT * 4 + CURSOR_PIXEL_COUNT / 8

#define VBOX_MAX_SCREENS  32

#define GUEST_HEAP_OFFSET(vbox) (vbox->full_vram_size - VBVA_ADAPTER_INFORMATION_SIZE)
#define GUEST_HEAP_SIZE   VBVA_ADAPTER_INFORMATION_SIZE
#define GUEST_HEAP_USABLE_SIZE (VBVA_ADAPTER_INFORMATION_SIZE - \
                                sizeof(HGSMIHOSTFLAGS))
#define HOST_FLAGS_OFFSET GUEST_HEAP_USABLE_SIZE

struct vbox_fbdev;

struct vbox_private {
    struct drm_device *dev;

    u8 __iomem *guest_heap;
    u8 __iomem *vbva_buffers;
    struct gen_pool *guest_pool;
    struct VBVABUFFERCONTEXT *vbva_info;
    bool any_pitch;
    unsigned num_crtcs;
    /** Amount of available VRAM, including space used for buffers. */
    uint32_t full_vram_size;
    /** Amount of available VRAM, not including space used for buffers. */
    uint32_t available_vram_size;
    /** Array of structures for receiving mode hints. */
    VBVAMODEHINT *last_mode_hints;

    struct vbox_fbdev *fbdev;

    int fb_mtrr;

    struct {
        struct drm_global_reference mem_global_ref;
        struct ttm_bo_global_ref bo_global_ref;
        struct ttm_bo_device bdev;
        bool mm_initialised;
    } ttm;

    struct mutex hw_mutex;
    bool isr_installed;
    /** We decide whether or not user-space supports display hot-plug
     * depending on whether they react to a hot-plug event after the initial
     * mode query. */
    bool initial_mode_queried;
    struct work_struct hotplug_work;
    uint32_t input_mapping_width;
    uint32_t input_mapping_height;
    /** Is user-space using an X.Org-style layout of one large frame-buffer
     * encompassing all screen ones or is the fbdev console active? */
    bool single_framebuffer;
    uint32_t cursor_width;
    uint32_t cursor_height;
    uint32_t cursor_hot_x;
    uint32_t cursor_hot_y;
    size_t cursor_data_size;
    uint8_t cursor_data[CURSOR_DATA_SIZE];
};

#undef CURSOR_PIXEL_COUNT
#undef CURSOR_DATA_SIZE

int vbox_driver_load(struct drm_device *dev, unsigned long flags);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
void vbox_driver_unload(struct drm_device *dev);
#else
int vbox_driver_unload(struct drm_device *dev);
#endif
void vbox_driver_lastclose(struct drm_device *dev);

struct vbox_gem_object;

#ifndef VGA_PORT_HGSMI_HOST
# define VGA_PORT_HGSMI_HOST             0x3b0
# define VGA_PORT_HGSMI_GUEST            0x3d0
#endif

struct vbox_connector {
    struct drm_connector base;
    char name[32];
    struct vbox_crtc *vbox_crtc;
    struct {
        uint16_t width;
        uint16_t height;
        bool disconnected;
    } mode_hint;
};

struct vbox_crtc {
    struct drm_crtc base;
    bool blanked;
    bool disconnected;
    unsigned crtc_id;
    uint32_t fb_offset;
    bool cursor_enabled;
    uint16_t x_hint;
    uint16_t y_hint;
};

struct vbox_encoder {
    struct drm_encoder base;
};

struct vbox_framebuffer {
    struct drm_framebuffer base;
    struct drm_gem_object *obj;
};

struct vbox_fbdev {
    struct drm_fb_helper helper;
    struct vbox_framebuffer afb;
    int size;
    struct ttm_bo_kmap_obj mapping;
    int x1, y1, x2, y2; /* dirty rect */
    spinlock_t dirty_lock;
};

#define to_vbox_crtc(x) container_of(x, struct vbox_crtc, base)
#define to_vbox_connector(x) container_of(x, struct vbox_connector, base)
#define to_vbox_encoder(x) container_of(x, struct vbox_encoder, base)
#define to_vbox_framebuffer(x) container_of(x, struct vbox_framebuffer, base)

extern int vbox_mode_init(struct drm_device *dev);
extern void vbox_mode_fini(struct drm_device *dev);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0)
# define DRM_MODE_FB_CMD drm_mode_fb_cmd
#else
# define DRM_MODE_FB_CMD drm_mode_fb_cmd2
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 15, 0)
# define CRTC_FB(crtc) (crtc)->fb
#else
# define CRTC_FB(crtc) (crtc)->primary->fb
#endif

void vbox_enable_accel(struct vbox_private *vbox);
void vbox_disable_accel(struct vbox_private *vbox);
void vbox_report_caps(struct vbox_private *vbox);

void vbox_framebuffer_dirty_rectangles(struct drm_framebuffer *fb,
                                       struct drm_clip_rect *rects,
                                       unsigned num_rects);

int vbox_framebuffer_init(struct drm_device *dev,
             struct vbox_framebuffer *vbox_fb,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 5, 0)
             const
#endif
             struct DRM_MODE_FB_CMD *mode_cmd,
             struct drm_gem_object *obj);

int vbox_fbdev_init(struct drm_device *dev);
void vbox_fbdev_fini(struct drm_device *dev);
void vbox_fbdev_set_suspend(struct drm_device *dev, int state);
void vbox_fbdev_set_base(struct vbox_private *vbox, unsigned long gpu_addr);

struct vbox_bo {
    struct ttm_buffer_object bo;
    struct ttm_placement placement;
    struct ttm_bo_kmap_obj kmap;
    struct drm_gem_object gem;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
    u32 placements[3];
#else
    struct ttm_place placements[3];
#endif
    int pin_count;
};
#define gem_to_vbox_bo(gobj) container_of((gobj), struct vbox_bo, gem)

static inline struct vbox_bo *
vbox_bo(struct ttm_buffer_object *bo)
{
    return container_of(bo, struct vbox_bo, bo);
}


#define to_vbox_obj(x) container_of(x, struct vbox_gem_object, base)

extern int vbox_dumb_create(struct drm_file *file,
               struct drm_device *dev,
               struct drm_mode_create_dumb *args);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 12, 0)
extern int vbox_dumb_destroy(struct drm_file *file,
                struct drm_device *dev,
                uint32_t handle);
#endif

extern void vbox_gem_free_object(struct drm_gem_object *obj);
extern int vbox_dumb_mmap_offset(struct drm_file *file,
                struct drm_device *dev,
                uint32_t handle,
                uint64_t *offset);

#define DRM_FILE_PAGE_OFFSET (0x10000000ULL >> PAGE_SHIFT)

int vbox_mm_init(struct vbox_private *vbox);
void vbox_mm_fini(struct vbox_private *vbox);

int vbox_bo_create(struct drm_device *dev, int size, int align,
          uint32_t flags, struct vbox_bo **pvboxbo);

int vbox_gem_create(struct drm_device *dev,
           u32 size, bool iskernel,
           struct drm_gem_object **obj);

int vbox_bo_pin(struct vbox_bo *bo, u32 pl_flag, u64 *gpu_addr);
int vbox_bo_unpin(struct vbox_bo *bo);

static inline int vbox_bo_reserve(struct vbox_bo *bo, bool no_wait)
{
    int ret;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0)
    ret = ttm_bo_reserve(&bo->bo, true, no_wait, NULL);
#else
    ret = ttm_bo_reserve(&bo->bo, true, no_wait, false, 0);
#endif
    if (ret)
    {
        if (ret != -ERESTARTSYS && ret != -EBUSY)
            DRM_ERROR("reserve failed %p\n", bo);
        return ret;
    }
    return 0;
}

static inline void vbox_bo_unreserve(struct vbox_bo *bo)
{
    ttm_bo_unreserve(&bo->bo);
}

void vbox_ttm_placement(struct vbox_bo *bo, int domain);
int vbox_bo_push_sysram(struct vbox_bo *bo);
int vbox_mmap(struct file *filp, struct vm_area_struct *vma);

/*vbox_prime*/
int vbox_gem_prime_pin(struct drm_gem_object *obj);
void vbox_gem_prime_unpin(struct drm_gem_object *obj);
struct sg_table *vbox_gem_prime_get_sg_table(struct drm_gem_object *obj);
struct drm_gem_object *vbox_gem_prime_import_sg_table(
        struct drm_device *dev,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
        size_t size,
#else
        struct dma_buf_attachment *attach,
#endif
        struct sg_table *table);
void *vbox_gem_prime_vmap(struct drm_gem_object *obj);
void vbox_gem_prime_vunmap(struct drm_gem_object *obj, void *vaddr);
int vbox_gem_prime_mmap(struct drm_gem_object *obj,
        struct vm_area_struct *area);

/* vbox_irq.c */
int vbox_irq_init(struct vbox_private *vbox);
void vbox_irq_fini(struct vbox_private *vbox);
void vbox_report_hotplug(struct vbox_private *vbox);
irqreturn_t vbox_irq_handler(int irq, void *arg);
#endif
