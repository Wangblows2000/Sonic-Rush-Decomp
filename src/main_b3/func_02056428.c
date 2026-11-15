#include <nitro/types.h>

typedef struct { u8 pad0[0xa]; u8 unkA; u8 padB[0xd]; s32 unk18; s32 unk1C; s32 unk20; u8 pad24[0x114]; } UnkObj_02056428;
typedef struct {
    u8 pad0[0x18];
    void **names;            /* 0x18 */
    u8 pad1C[4];
    s8 count;                /* 0x20 */
    s8 unk21;
    u8 pad22[2];
    u8 *arc;                 /* 0x24 */
    void *anm[5];            /* 0x28 */
    UnkObj_02056428 objs[15];/* 0x3c */
    void *res[15];           /* 0x1284 */
    u32 flags[15];           /* 0x12c0 */
} UnkWork_02056428;
typedef struct { u8 data[0x5c]; } UnkArc_02056428;

extern void *data_0208fe80;
extern char data_0207ba98[];
extern s32 data_02075704[];

u8 *func_02033fbc(void *path, s32 b);
u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
s32 func_02056834(void *p);
u32 func_02056828(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *p);
void func_02033a68(void *src, void *dst, u32 size);
BOOL func_02021310(UnkArc_02056428 *arc, const char *name, void *narc);
void *func_02021264(UnkArc_02056428 *arc, s32 i);
BOOL func_020212e0(UnkArc_02056428 *arc);
void func_02057044(void *name, char *buf);
u32 func_02057038(void *name);
void func_02031768(UnkObj_02056428 *o);
void func_0202566c(void *res);
void func_02031c1c(UnkObj_02056428 *o, s32 a);
void func_020257f8(void *res);
void func_02031bd0(UnkObj_02056428 *o, void *res, s32 a);
void *func_02025e8c(void *res);
void func_02031a68(UnkObj_02056428 *o, s32 a, void *b, s32 c, void *d);

#pragma thumb on
static inline u8 *Alloc_02056428(u32 size) {
    u8 *p = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void Free_02056428(u8 *blk) {
    if ((u8 *)func_02006abc(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    func_020349b8(data_0208fe80, blk);
}

void func_02056428(UnkWork_02056428 *w) {
    u8 i;

    for (i = 0; i < w->count; i++) {
        func_02031768(&w->objs[i]);
        if (w->res[i] != NULL && !(w->flags[i] & 1)) {
            func_0202566c(w->res[i]);
            Free_02056428(w->res[i]);
        }
        w->res[i] = NULL;
        w->flags[i] = 0;
    }
    if (w->arc != NULL) {
        Free_02056428(w->arc);
        w->arc = NULL;
    }
}
#pragma thumb reset
