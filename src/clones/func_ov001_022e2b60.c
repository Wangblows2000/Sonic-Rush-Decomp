#include <nitro/types.h>

typedef struct {
    s32 t;
    s32 b;
    s32 l;
    s32 r;
    s32 n;
    s32 f;
} UnkFrustum_02356994;

typedef struct {
    u8 pad0[0x7C];
    u32 unk7C;
} UnkStruct_02084db8;

extern u8 data_02084d40[];
extern UnkStruct_02084db8 data_02084db8;

void func_0200468c(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 scaleW, BOOL draw, void *mtx);

#pragma thumb on

void func_ov001_022e2b60(UnkFrustum_02356994 *p) {
    func_0200468c(p->t, p->b, p->l, p->r, p->n, p->f, 0x1000, FALSE, data_02084d40);
    data_02084db8.unk7C &= ~0x50;
}

#pragma thumb reset
