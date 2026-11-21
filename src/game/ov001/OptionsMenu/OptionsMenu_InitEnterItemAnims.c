#include <nitro/types.h>

typedef struct {
    u8 data[0x2c];
} UnkAnim_022cb770;

typedef struct {
    u32 type;
    UnkAnim_022cb770 anim;
} UnkItem_022cb770;

typedef struct {
    s32 unk0;
    u8 pad4[0x2c];
    s32 tbl[(0x1e2c - 0x30) / 4];
    s32 res;
} UnkCfg_022cb770;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0xc];
    s32 unk14;
    u8 pad18[0x34 - 0x18];
    s32 unk34;
    u8 pad38[0x208 - 0x38];
    UnkAnim_022cb770 animB;
    UnkAnim_022cb770 animA;
    UnkAnim_022cb770 animC;
    UnkItem_022cb770 items[6];
    UnkCfg_022cb770 *cfg;
} UnkStruct_022cb770;

void KeyframeAnim_Init(void *p, s32 a, s32 b);
void KeyframeAnim_SetToLastFrame(void *p);

#pragma thumb on
void OptionsMenu_InitEnterItemAnims(UnkStruct_022cb770 *p) {
    UnkCfg_022cb770 *cfg = p->cfg;
    s32 res = cfg->res;
    s32 *tbl = cfg->tbl;
    s32 i;

    KeyframeAnim_Init(&p->animA, res, tbl[14]);
    KeyframeAnim_Init(&p->animB, res, tbl[12]);
    KeyframeAnim_SetToLastFrame(&p->animB);
    KeyframeAnim_Init(&p->animC, res, tbl[16]);
    for (i = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (i == p->cur) {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl[4]);
            KeyframeAnim_SetToLastFrame(&p->items[i].anim);
        } else {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl[5]);
            KeyframeAnim_SetToLastFrame(&p->items[i].anim);
        }
    }
    cfg->unk0 = 4;
    p->unk34 = -1;

}
#pragma thumb reset
