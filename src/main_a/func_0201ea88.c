#include <nitro/types.h>

typedef struct { u32 unk0; u16 unk4; u16 unk6; u8 unk8; u8 unk9; } UnkEntry_0201ea88;
u32 *func_0201d46c(s32 idx);
void *func_0201d0f8(u32 idx);
UnkEntry_0201ea88 *func_020204f4(void *p, s32 idx);
u32 func_0201e7f0(void *a, u8 b, u16 c, u8 d, UnkEntry_0201ea88 *e, void *f, s32 g, s32 h);

u32 func_0201ea88(void *a, s32 idx, s32 sub) {
    u32 *info;
    void *p;
    UnkEntry_0201ea88 *e;
    info = func_0201d46c(idx);
    if (info == NULL) return 0;
    p = func_0201d0f8(*info);
    if (p == NULL) return 0;
    e = func_020204f4(p, sub);
    if (e == NULL) return 0;
    return func_0201e7f0(a, e->unk9, e->unk4, e->unk8, e, p, idx, sub);
}
