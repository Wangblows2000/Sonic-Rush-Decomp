#include <nitro/types.h>

typedef struct {
    void *p0;
    void *p1;
} UnkPair_02337338;

typedef struct {
    u8 pad0[0x5C];
    UnkPair_02337338 pal;
    u8 pad64[0x7C - 0x64];
} UnkObj_02337338;

typedef struct {
    u8 pad0[4];
    UnkObj_02337338 obj[5];
} UnkGrpC_02337338;

typedef struct {
    u8 pad0[8];
    UnkObj_02337338 obj[2];
} UnkGrpD_02337338;

typedef struct {
    u8 pad0[0xC];
    UnkPair_02337338 pairC;
    UnkPair_02337338 pair14;
    u8 pad1C[4];
    UnkObj_02337338 objA[2];
    u8 pad118[0x29C - 0x118];
    UnkObj_02337338 obj29C;
    u8 pad318[0x584 - 0x318];
    UnkObj_02337338 objB[2];
    u8 pad67C[0x688 - 0x67C];
    UnkGrpC_02337338 grpC;
    u8 pad8F8[0x924 - 0x8F8];
    UnkGrpD_02337338 grpD;
} UnkStruct_02337338;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02337338 *work;
} UnkStruct_02337338_Task;

extern void *data_ov000_0236aaa8;

void func_02039bb0(s32 a, void *p);

static inline void UnkRelease_02337338(UnkPair_02337338 *p) {
    if (p->p0 != NULL) {
        func_02039bb0(0, p->p0);
    }
    if (p->p1 != NULL) {
        func_02039bb0(1, p->p1);
    }
}


void func_ov000_02337338(UnkStruct_02337338_Task *t) {
    UnkStruct_02337338 *w = t->work;
    u32 i;
    UnkObj_02337338 *o;
    UnkGrpC_02337338 *gc;
    UnkGrpD_02337338 *gd;

    UnkRelease_02337338(&w->pairC);
    UnkRelease_02337338(&w->pair14);
    o = &w->obj29C;
    UnkRelease_02337338(&o->pal);
    for (i = 0, o = w->objB; i < 2; i++, o++) {
        UnkRelease_02337338(&o->pal);
    }
    o = &w->objA[0];
    UnkRelease_02337338(&o->pal);
    o = &w->objA[1];
    UnkRelease_02337338(&o->pal);
    gc = &w->grpC;
    o = &gc->obj[0];
    UnkRelease_02337338(&o->pal);
    o = &gc->obj[1];
    UnkRelease_02337338(&o->pal);
    o = &gc->obj[2];
    UnkRelease_02337338(&o->pal);
    o = &gc->obj[3];
    UnkRelease_02337338(&o->pal);
    o = &gc->obj[4];
    UnkRelease_02337338(&o->pal);
    gd = &w->grpD;
    o = &gd->obj[0];
    UnkRelease_02337338(&o->pal);
    o = &gd->obj[1];
    UnkRelease_02337338(&o->pal);
    data_ov000_0236aaa8 = NULL;
}
