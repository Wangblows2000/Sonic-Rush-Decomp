#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u32 flags;
    u8 pad14[0x18];
} UnkAnim_022cb834;

typedef struct {
    u32 type;
    UnkAnim_022cb834 anim;
} UnkItem_022cb834;

typedef struct {
    u8 pad0[4];
    s32 sel;
    s32 unsel;
    u8 padc[0x28 - 0xc];
    s32 animB;
    u8 pad2c[0x3c - 0x2c];
    s32 animA;
    u8 pad40[4];
    s32 animC;
} UnkTbl_022cb834;

typedef struct {
    s32 unk0;
    u8 pad4[0x2c];
    UnkTbl_022cb834 tbl;
    u8 pad[0x1e2c - 0x30 - sizeof(UnkTbl_022cb834)];
    s32 res;
} UnkCfg_022cb834;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0x208 - 8];
    UnkAnim_022cb834 animB;
    UnkAnim_022cb834 animA;
    UnkAnim_022cb834 animC;
    UnkItem_022cb834 items[6];
    UnkCfg_022cb834 *cfg;
} UnkStruct_022cb834;

void KeyframeAnim_Init(void *p, s32 a, s32 b);
void KeyframeAnim_PlayReverse(void *p);

#pragma thumb on
void Options_InitExitAnims(UnkStruct_022cb834 *p) {
    UnkCfg_022cb834 *cfg = p->cfg;
    s32 res = cfg->res;
    UnkTbl_022cb834 *tbl = &cfg->tbl;
    s32 k;
    s32 i;

    KeyframeAnim_Init(&p->animA, res, tbl->animA);
    KeyframeAnim_PlayReverse(&p->animA);
    p->animA.flags |= 0x10;
    KeyframeAnim_Init(&p->animB, res, tbl->animB);
    KeyframeAnim_PlayReverse(&p->animB);
    p->animB.flags |= 0x10;
    KeyframeAnim_Init(&p->animC, res, tbl->animC);
    KeyframeAnim_PlayReverse(&p->animC);
    p->animC.flags |= 0x10;
    for (i = 0, k = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (p->cur == i) {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl->sel + k);
            KeyframeAnim_PlayReverse(&p->items[i].anim);
            p->items[i].anim.flags |= 0x10;
        } else {
            KeyframeAnim_Init(&p->items[i].anim, res, tbl->unsel + k);
            KeyframeAnim_PlayReverse(&p->items[i].anim);
            p->items[i].anim.flags |= 0x10;
        }
        k++;
    }
    cfg->unk0 = 7;
}
#pragma thumb reset
