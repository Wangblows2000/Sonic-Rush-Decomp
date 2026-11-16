#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022d6bf0;

typedef struct {
    u8 pad0[0x29c];
    UnkSpr_022d6bf0 spr[4];
    s32 state[4];
    u8 pad3cc[0x490 - 0x3cc];
    void *res0;
    void *res1;
} UnkStruct_022d6bf0;

void func_ov001_022d6864(UnkSpr_022d6bf0 *s, void *res, u16 idx, s32 mode, u16 a, u16 b);

#pragma thumb on
static inline void Setup_022d6bf0(UnkSpr_022d6bf0 *s, void *res, u16 idx, u16 a, u16 b) {
    func_ov001_022d6864(s, res, idx, 1, a, b);
}

void func_ov001_022d6bf0(UnkStruct_022d6bf0 *p) {
    UnkSpr_022d6bf0 *s;
    void *r;
    s = &p->spr[0];
    r = p->res0;
    func_ov001_022d6864(s, r, 0, 1, 0, 0xc);
    p->state[0] = 0;
    s = &p->spr[1];
    r = p->res0;
    func_ov001_022d6864(s, r, 1, 1, 1, 0x10);
    p->state[1] = 1;
    s = &p->spr[2];
    r = p->res1;
    func_ov001_022d6864(s, r, 0, 1, 2, 8);
    p->state[2] = 0;
    s = &p->spr[3];
    r = p->res1;
    func_ov001_022d6864(s, r, 1, 1, 3, 4);
    p->state[3] = 1;
}
#pragma thumb reset
