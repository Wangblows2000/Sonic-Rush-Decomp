#include <nitro/types.h>

typedef struct UnkStruct_02309c14_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkStruct_02309c14_o;

typedef struct UnkStruct_02309c14 {
    u8 pad0[0x108];
    UnkStruct_02309c14_o objs[35];
    s32 unkAE0;
    u8 padAE4[0xaf0 - 0xae4];
    s32 unkAF0;
    u8 padAF4[0xba8 - 0xaf4];
    s32 unkBA8;
    u8 padBAC[4];
    s32 unkBB0;
    s32 unkBB4;
    s32 unkBB8;
    s32 unkBBC;
    u8 padBC0[4];
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
    s32 unkBD0;
    s32 unkBD4;
} UnkStruct_02309c14;

extern UnkStruct_02309c14 *data_ov001_0231f5bc;

void func_ov001_02307b44(void);
void AnimatorSprite_Draw(void *p);
u32 func_ov001_0230c0b8(void);
void func_ov001_02308884(void);
void func_ov001_023091a0(void);
void func_ov001_02308fc8(void);
void func_ov001_02308e44(void);
void func_ov001_02308d40(void);
void func_ov001_02308720(void);
void func_ov001_02308600(void);

void func_ov001_02309c14(void) {
    UnkStruct_02309c14 *g = data_ov001_0231f5bc;
    s32 i;

    if (g->unkBD4 >= 0) {
        func_ov001_02307b44();
        return;
    }
    AnimatorSprite_Draw(&g->objs[0]);
    AnimatorSprite_Draw(&g->objs[1]);
    g->objs[8].flags |= 0x80;
    AnimatorSprite_Draw(&g->objs[8]);
    g->objs[8].flags &= ~0x80;
    AnimatorSprite_Draw(&g->objs[8]);
    if (g->unkBB8 > 0 && ((g->unkBB8 & 0x20) || g->unkBBC == 0))
        AnimatorSprite_Draw(&g->objs[9]);
    for (i = 0; i < 7; i++) {
        u32 m = func_ov001_0230c0b8();
        if (m & (1 << i)) {
            AnimatorSprite_Draw(&g->objs[0xb + i]);
        } else {
            g->objs[10].x = g->objs[0xb + i].x;
            g->objs[10].y = g->objs[0xb + i].y;
            AnimatorSprite_Draw(&g->objs[10]);
        }
    }
    if (g->unkBA8 != 0)
        AnimatorSprite_Draw(&g->objs[18]);
    if (g->unkBB0 != 0) {
        if (g->unkBB4 != 0) {
            g->objs[20].x = 0xe8;
            g->objs[20].y = 0xac;
        } else {
            g->objs[20].x = 0x80;
            g->objs[20].y = 0xac;
        }
        AnimatorSprite_Draw(&g->objs[20]);
    }
    func_ov001_02308884();
    if (g->unkAF0 > 0)
        AnimatorSprite_Draw(&g->objs[24]);
    if (g->unkBC4 >= 0)
        func_ov001_023091a0();
    if (g->unkBC8 >= 0) {
        func_ov001_02308fc8();
        return;
    }
    if (g->unkBCC >= 0) {
        func_ov001_02308e44();
        return;
    }
    if (g->unkBD0 >= 0)
        func_ov001_02308d40();
    func_ov001_02308720();
    func_ov001_02308600();
    if (g->unkAE0 >= 0x17c && g->unkAE0 < 0x258)
        AnimatorSprite_Draw(&g->objs[34]);
}
