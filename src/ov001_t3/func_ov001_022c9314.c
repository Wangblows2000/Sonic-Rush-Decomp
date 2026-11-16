#include <nitro/types.h>

typedef struct {
    u8 pad0[0x29];
    u8 unk29;
    u8 type;
    u8 unk2b;
    u8 unk2c;
    u8 pad2d;
    u16 id;
} UnkEntry_022c9314;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padc[0x18 - 0xc];
    s32 idx;
    u8 pad1c[0x180 - 0x1c];
    s32 unk180;
    u8 pad184[0x1a4 - 0x184];
    u8 unk1a4[4];
} UnkStruct_022c9314;

extern UnkEntry_022c9314 *data_ov001_0231f048;

BOOL func_ov001_022c9310(UnkStruct_022c9314 *p);
void func_0201cf04(void);
void func_0201b348(void);
void func_02020230(void *a, u16 id, s32 b);
void func_020367b4(s32 a, u16 id);
void func_02036754(s32 a, u8 b, u16 id);
void func_020366f4(s32 a, u8 b, u16 id);
void func_0201cf4c(s32 a, s32 b, s32 c, s32 d, s32 e, void (*fn)(void *), void *arg);
void func_ov001_022c8f30(void *p);

#pragma thumb on
void func_ov001_022c9314(UnkStruct_022c9314 *p) {
    UnkEntry_022c9314 *e;

    if (!func_ov001_022c9310(p)) return;
    func_0201cf04();
    e = &data_ov001_0231f048[p->idx];
    func_0201b348();
    switch (e->type) {
    case 0:
        if (e->unk29 == 0 && e->unk2b == 0xe) {
            func_02020230(p->unk1a4, e->id, 0);
            p->unk4 = 1;
        } else {
            func_020367b4(0, e->id);
            p->unk4 = 0;
        }
        break;
    case 1:
        func_02036754(0, e->unk2c, e->id);
        p->unk4 = 0;
        break;
    case 2:
        func_020366f4(0, e->unk2c, e->id);
        p->unk4 = 0;
        break;
    }
    if (p->unk8 != 0) {
        func_0201cf4c(p->unk180, 0x80, 1, 0x1f40, 2, func_ov001_022c8f30, p);
    }
}
#pragma thumb reset
