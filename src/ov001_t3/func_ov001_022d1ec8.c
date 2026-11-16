#include <nitro/types.h>

typedef struct {
    u8 pad0[0x40];
} UnkObj_022d1ec8;

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022d1ec8;

typedef struct {
    s32 mode;
    u8 pad4[0x14 - 4];
    s32 timer;
    u8 pad18[0x54 - 0x18];
    UnkObj_022d1ec8 objs[(0x420 - 0x54) / 0x40];
    u8 pad414[0x420 - 0x54 - ((0x420 - 0x54) / 0x40) * 0x40];
    UnkSpr_022d1ec8 spr[18];
    u8 pada50[0xbb0 - 0x930];
    s32 unkbb0;
    s32 unkbb4;
    u8 padbb8[0xbe0 - 0xbb8];
    s32 unkbe0;
    u8 padbe4[0xd0c - 0xbe4];
    u8 unkd0c[0xf0c - 0xd0c];
    u8 unkf0c[4];
} UnkStruct_022d1ec8;

void AnimatorSprite_Draw(UnkSpr_022d1ec8 *s);
void StaffRoll_SetState(UnkStruct_022d1ec8 *p, void (*fn)(UnkStruct_022d1ec8 *));
void func_ov001_022cff74(UnkObj_022d1ec8 *o, s32 a, void *b, void *c);
void func_ov001_022d1904(UnkStruct_022d1ec8 *p);
void func_ov001_022d1e00(UnkStruct_022d1ec8 *p);

#pragma thumb on
void func_ov001_022d1ec8(UnkStruct_022d1ec8 *p) {
    s32 i;

    if (p->mode != 2 && p->unkbb4 == 0) {
        p->unkbb0 = 0;
        p->unkbb4 = 0;
    }
    for (i = 0; i < 18; i++) {
        if (i * 3 + 5 <= p->timer) {
            AnimatorSprite_Draw(&p->spr[i]);
        }
    }
    if (p->timer > 0xf0) {
        p->unkbe0 = 1;
        if (p->mode == 2) {
            *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1800;
            *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1800;
            *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 0x18c;
            StaffRoll_SetState(p, func_ov001_022d1904);
            return;
        }
        *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1800;
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1800;
        *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0x18c;
        func_ov001_022cff74(&p->objs[2], 0x100, p->unkf0c, p->unkd0c);
        StaffRoll_SetState(p, func_ov001_022d1e00);
    }
}
#pragma thumb reset
