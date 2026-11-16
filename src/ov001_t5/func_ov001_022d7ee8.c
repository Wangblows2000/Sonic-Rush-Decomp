#include <nitro/types.h>

typedef struct {
    s32 id;
    u8 pad4[4];
    u8 obj[0x3c - 8];
} UnkEntry_022d7ee8;

typedef struct {
    s16 h[8];
    s32 unk10;
    UnkEntry_022d7ee8 e[6];
} UnkStruct_022d7ee8;

typedef struct {
    u8 pad0[0x828];
    void *res;
} UnkCfg_022d7ee8;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02040ef0(void *obj, void *res, s32 idx);
void func_02040e80(void *obj);

#pragma thumb on
void func_ov001_022d7ee8(UnkStruct_022d7ee8 *p, s32 id, s32 a, s32 b, s32 c, s32 d, UnkCfg_022d7ee8 *cfg) {
    s32 n;
    s32 i;
    s32 off;
    s16 h;
    UnkEntry_022d7ee8 *e;

    p->h[0] = 0x40;
    p->h[1] = 0x20;
    p->h[2] = 0;
    p->h[3] = 0x20;
    p->h[4] = 0x10;
    p->h[5] = 0x4c;
    p->h[6] = 0x40;
    p->h[7] = 0x90;
    p->unk10 = 1;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, p->e, sizeof(p->e));
    }
    i = 0;
    if (a != 0x14) {
        p->e[i].id = a;
        i++;
    }
    if (b != 0x14) {
        p->e[i].id = b;
        i++;
    }
    if (c != 0x14) {
        p->e[i].id = c;
        i++;
    }
    if (d != 0x14) {
        p->e[i].id = d;
        i++;
    }
    n = i;
    for (; i < 4; i++) {
        p->e[i].id = 0x14;
    }
    switch (n) {
    case 0:
        break;
    case 1:
        h = p->h[3];
        p->h[1] += (s16)(h / 2) + h;
        break;
    case 2:
        p->h[1] += p->h[3];
        break;
    case 3:
        p->h[1] += (s16)(p->h[3] / 2);
        break;
    case 4:
        break;
    }
    e = &p->e[5];
    e->id = id;
    func_02040ef0(e->obj, cfg->res, 6);
    func_02040e80(e->obj);
    e = &p->e[4];
    e->id = 0;
    func_02040ef0(e->obj, cfg->res, 0xb);
    func_02040e80(e->obj);
    i = 0;
    e = p->e;
    off = (4 - n) / 2;
    for (; i < 4; e++, i++) {
        if (e->id < 0x13) {
            func_02040ef0(e->obj, cfg->res, i + off + 7);
            func_02040e80(e->obj);
        }
    }
}
#pragma thumb reset
