#include <nitro/types.h>

typedef struct { u8 pad0[0xa]; u8 unkA; u8 padB[0xd]; s32 unk18; s32 unk1C; s32 unk20; u8 pad24[0x114]; } UnkObj_0205652c;
typedef struct {
    u8 pad0[0x18];
    void **names;            /* 0x18 */
    u8 pad1C[4];
    s8 count;                /* 0x20 */
    s8 unk21;
    u8 pad22[2];
    u8 *arc;                 /* 0x24 */
    void *anm[5];            /* 0x28 */
    UnkObj_0205652c objs[15];/* 0x3c */
    void *res[15];           /* 0x1284 */
    u32 flags[15];           /* 0x12c0 */
} UnkWork_0205652c;
typedef struct { u8 data[0x5c]; } UnkArc_0205652c;

extern void *gMainHeap;
extern char data_0207ba98[];
extern s32 data_02075704[];

u8 *File_Load(void *path, s32 b);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
s32 func_02056834(void *p);
u32 func_02056828(void *p);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *p);
void Mem_Copy(void *src, void *dst, u32 size);
BOOL func_02021310(UnkArc_0205652c *arc, const char *name, void *narc);
void *func_02021264(UnkArc_0205652c *arc, s32 i);
BOOL func_020212e0(UnkArc_0205652c *arc);
void PolyDemo_BuildModelPath(void *name, char *buf);
u32 func_02057038(void *name);
void Model_Init(UnkObj_0205652c *o, s32 a);
void func_020257f8(void *res);
void Model_SetResource(UnkObj_0205652c *o, void *res, s32 a);
void *func_02025e8c(void *res);
void Model_SetAnimation(UnkObj_0205652c *o, s32 a, void *b, s32 c, void *d);

#pragma thumb on
static inline u8 *Alloc_0205652c(u32 size) {
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
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

static inline void Free_0205652c(u8 *blk) {
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
    Heap_Free(gMainHeap, blk);
}

void func_0205652c(UnkWork_0205652c *w) {
    u8 *f;
    u8 *p;
    u8 i;
    u8 j;
    u8 k;
    void **names;
    UnkObj_0205652c *obj;

    f = File_Load(w->names[0], -1);
    w->arc = Alloc_0205652c(((*(u32 *)f >> 8) + 0x43) & ~0x1f);
    p = w->arc;
    switch (func_02056834(f)) {
    case 0x10:
        func_02008648(f, p);
        break;
    case 0x20:
        func_02008720(f, p);
        break;
    case 0x30:
        func_020087f8(f, p);
        break;
    default:
        Mem_Copy(Compress_GetData(f), p, func_02056828(f));
        break;
    }
    DC_StoreRange(p, func_02056828(f));
    Free_0205652c(f);
    {
        UnkArc_0205652c arc;
        func_02021310(&arc, data_0207ba98, w->arc);
        for (i = 0; i < 5; i++)
            w->anm[i] = func_02021264(&arc, i);
        w->res[0] = func_02021264(&arc, 5);
        w->flags[0] |= 5;
        w->res[1] = func_02021264(&arc, 5);
        w->flags[1] |= 5;
        func_020212e0(&arc);
    }
    {
        char buf[0x30] = {0};
        names = (void **)w->names[1];
        w->unk21 = -1;
        for (i = 2; i < w->count; i++) {
            PolyDemo_BuildModelPath(names[i], buf);
            w->res[i] = File_Load(buf, -1);
            w->flags[i] = func_02057038(names[i]);
            if (w->flags[i] & 2)
                w->unk21 = i;
        }
    }
    for (j = 0; j < w->count; j++) {
        obj = &w->objs[j];
        Model_Init(obj, 0);
        obj->unkA = 4;
        func_020257f8(w->res[j]);
        if (w->flags[j] & 1)
            Model_SetResource(obj, w->res[j], j);
        else
            Model_SetResource(obj, w->res[j], 0);
        if (w->flags[j] & 2) {
            obj->unk20 = 0x3ddf10;
            obj->unk1C = obj->unk20;
            obj->unk18 = obj->unk1C;
        }
        if (w->flags[j] & 4) {
            void *m = func_02025e8c(w->res[j]);
            for (k = 0; k < 5; k++) {
                if (w->flags[j] & 1) {
                    if (k != 0 && k != 1)
                        continue;
                }
                {
                    if ((s8)data_02075704[k] >= 0) {
                        if ((u8)(k + 0xfd) <= 1) {
                            if (m != NULL)
                                Model_SetAnimation(obj, *(s32 *)&data_02075704[k], w->anm[k], j, m);
                        } else {
                            Model_SetAnimation(obj, *(s32 *)&data_02075704[k], w->anm[k], j, NULL);
                        }
                    }
                }
            }
        }
    }
}
#pragma thumb reset
