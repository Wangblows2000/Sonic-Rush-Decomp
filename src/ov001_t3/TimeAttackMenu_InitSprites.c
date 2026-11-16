#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022ebecc;

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022ebecc;

typedef struct {
    u8 pad0[4];
    s32 mode;
    u8 pad8[0x228 - 8];
    UnkSpr_022ebecc spr[0x1f];
    s32 anim[0x1f];
    UnkAnim_022ebecc anims[12];
    u8 padd6c[0x1080 - 0xd6c];
    void *res[9];
} UnkStruct_022ebecc;

extern u16 data_ov001_023166cc[];

void func_ov001_022e9f04(UnkSpr_022ebecc *s, void *res, u16 id, s32 a);
void func_ov001_022e9f58(UnkSpr_022ebecc *s, void *res, u16 id, s32 a);
void KeyframeAnim_Init(UnkAnim_022ebecc *a, void *res, s32 i);

#pragma thumb on
void TimeAttackMenu_InitSprites(UnkStruct_022ebecc *p) {
    s32 i;
    void *res;
    s32 id;
    UnkSpr_022ebecc *s;

    for (i = 0; i < 0x1f; i++) {
        s = &p->spr[i];
        if (i == 0) {
            res = p->res[0];
        } else if (i == 0x14) {
            res = p->res[1];
        } else if (i == 0x18) {
            res = p->res[2];
        } else if (i == 0x19) {
            if (p->mode == 0 || p->mode == 2) {
                res = p->res[3];
            } else {
                res = p->res[4];
            }
        } else if (i == 0x1a) {
            res = p->res[5];
        } else if (i == 0x1c) {
            res = p->res[6];
        } else if (i == 0x1d) {
            res = p->res[8];
        }
        id = data_ov001_023166cc[i];
        p->anim[i] = -1;
        if (i == 0x18) {
            func_ov001_022e9f04(s, res, id, 0);
        } else {
            func_ov001_022e9f58(s, res, id, 1);
        }
    }
    for (i = 0; i < 12; i++) {
        KeyframeAnim_Init(&p->anims[i], p->res[7], 0);
    }
}
#pragma thumb reset
