/* host stub of the KOS PVR API: just what engine/render.c compiles against (test_palette.c) */
#ifndef STUB_DC_PVR_H
#define STUB_DC_PVR_H
#include <stdint.h>
#include <stddef.h>
typedef int pvr_list_t;
typedef void *pvr_ptr_t;
typedef struct { struct { int culling; } gen; } pvr_poly_cxt_t;
typedef struct { uint32_t d[8]; } pvr_poly_hdr_t;
typedef struct { uint32_t flags; float x, y, z, u, v; uint32_t argb, oargb; } pvr_vertex_t;
enum { PVR_LIST_OP_POLY, PVR_LIST_OM_POLY, PVR_LIST_TR_POLY, PVR_LIST_TM_POLY, PVR_LIST_PT_POLY };
#define PVR_CMD_VERTEX 0xe0000000u
#define PVR_CMD_VERTEX_EOL 0xf0000000u
#define PVR_FILTER_NONE 0
#define PVR_CULLING_NONE 0
#define PVR_PAL_ARGB1555 0
#define PVR_TXRFMT_ARGB1555 0
#define PVR_TXRFMT_RGB565 (1 << 27)
#define PVR_TXRFMT_ARGB4444 (2 << 27)
#define PVR_TXRFMT_PAL4BPP (5 << 27)
#define PVR_TXRFMT_PAL8BPP (6 << 27)
#define PVR_TXRFMT_4BPP_PAL(x) ((x) << 21)
#define PVR_TXRFMT_8BPP_PAL(x) ((x) << 25)
#define PVR_TXRFMT_TWIDDLED 0
#define PVR_TXRFMT_NONTWIDDLED (1 << 26)
#define PVR_TXRLOAD_4BPP 0
void pvr_set_pal_entry(uint32_t i, uint32_t v);
void pvr_set_pal_format(int f);
int pvr_wait_ready(void);
int pvr_wait_render_done(void);
int pvr_scene_begin(void);
int pvr_scene_finish(void);
int pvr_list_begin(pvr_list_t l);
int pvr_list_finish(void);
void pvr_poly_cxt_txr(pvr_poly_cxt_t *c, pvr_list_t l, int f, int w, int h, pvr_ptr_t p, int filt);
void pvr_poly_cxt_col(pvr_poly_cxt_t *c, pvr_list_t l);
void pvr_poly_compile(pvr_poly_hdr_t *h, pvr_poly_cxt_t *c);
int pvr_prim(void *d, int size);
pvr_ptr_t pvr_mem_malloc(size_t n);
void pvr_mem_free(pvr_ptr_t p);
uint32_t pvr_mem_available(void);
void pvr_txr_load(const void *src, pvr_ptr_t dst, uint32_t n);
#endif
