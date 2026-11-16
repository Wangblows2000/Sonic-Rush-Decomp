#include <nitro/types.h>

typedef struct {
    u8 data[0x2c];
} UnkAnim_022cba1c;

typedef struct {
    u32 type;
    UnkAnim_022cba1c anim;
} UnkItem_022cba1c;

typedef struct {
    s32 unk0;
    u8 pad4[0x2c];
    s32 tbl[(0x1e2c - 0x30) / 4];
    s32 res;
} UnkCfg_022cba1c;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0xc];
    s32 unk14;
    u8 pad18[0x208 - 0x18];
    UnkAnim_022cba1c animB;
    UnkAnim_022cba1c animA;
    UnkAnim_022cba1c animC;
    UnkItem_022cba1c items[6];
    UnkCfg_022cba1c *cfg;
} UnkStruct_022cba1c;

void KeyframeAnim_Init(void *p, s32 a, s32 b);
void KeyframeAnim_SetToLastFrame(void *p);

#pragma thumb on
void OptionsMenu_Unk_ov001_022cba1c(UnkStruct_022cba1c *p) {
    UnkCfg_022cba1c *cfg = p->cfg;
    s32 res = cfg->res;
    s32 *tbl = cfg->tbl;
    s32 i;

    KeyframeAnim_Init(&p->animA, res, tbl[14]);
    KeyframeAnim_Init(&p->animB, res, tbl[13]);
    KeyframeAnim_Init(&p->animC, res, tbl[16]);
    for (i = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (i == p->cur) {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl[7]);

        } else {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl[8]);
        }
    }
    cfg->unk0 = 5;

}
#pragma thumb reset
