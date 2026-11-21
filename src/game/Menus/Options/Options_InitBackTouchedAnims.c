#include <nitro/types.h>

typedef struct {
    u8 data[0x2c];
} UnkAnim_022cb96c;

typedef struct {
    u32 type;
    UnkAnim_022cb96c anim;
} UnkItem_022cb96c;

typedef struct {
    s32 unsel;
    u8 pad4[8];
    s32 sel;
    u8 pad10[0x24 - 0x10];
    s32 animB;
    u8 pad28[0x38 - 0x28];
    s32 animA;
    u8 pad3c[4];
    s32 animC;
} UnkTbl_022cb96c;

typedef struct {
    s32 unk0;
    u8 pad4[0x2c];
    UnkTbl_022cb96c tbl;
    u8 pad[0x1e2c - 0x30 - sizeof(UnkTbl_022cb96c)];
    s32 res;
} UnkCfg_022cb96c;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0xc];
    s32 unk14;
    u8 pad18[0x208 - 0x18];
    UnkAnim_022cb96c animB;
    UnkAnim_022cb96c animA;
    UnkAnim_022cb96c animC;
    UnkItem_022cb96c items[6];
    UnkCfg_022cb96c *cfg;
} UnkStruct_022cb96c;

void KeyframeAnim_Init(void *p, s32 a, s32 b);
void KeyframeAnim_SetToLastFrame(void *p);

#pragma thumb on
void Options_InitBackTouchedAnims(UnkStruct_022cb96c *p) {
    UnkCfg_022cb96c *cfg = p->cfg;
    s32 res = cfg->res;
    UnkTbl_022cb96c *tbl = &cfg->tbl;
    s32 i;

    KeyframeAnim_Init(&p->animA, res, tbl->animA);
    KeyframeAnim_Init(&p->animB, res, tbl->animB);
    KeyframeAnim_Init(&p->animC, res, tbl->animC);
    for (i = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (i == p->cur) {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl->sel);
            KeyframeAnim_SetToLastFrame(&p->items[i].anim);
        } else {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl->unsel);
        }
    }
    cfg->unk0 = 6;
    p->unk14 = 0;
}
#pragma thumb reset
