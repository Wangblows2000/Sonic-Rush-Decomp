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

void func_02040ef0(void *p, s32 a, s32 b);
void func_02040e80(void *p);

#pragma thumb on
void func_ov001_022cb834(UnkStruct_022cb834 *p) {
    UnkCfg_022cb834 *cfg = p->cfg;
    s32 res = cfg->res;
    UnkTbl_022cb834 *tbl = &cfg->tbl;
    s32 k;
    s32 i;

    func_02040ef0(&p->animA, res, tbl->animA);
    func_02040e80(&p->animA);
    p->animA.flags |= 0x10;
    func_02040ef0(&p->animB, res, tbl->animB);
    func_02040e80(&p->animB);
    p->animB.flags |= 0x10;
    func_02040ef0(&p->animC, res, tbl->animC);
    func_02040e80(&p->animC);
    p->animC.flags |= 0x10;
    for (i = 0, k = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (p->cur == i) {
            func_02040ef0(&p->items[i].anim, res, tbl->sel + k);
            func_02040e80(&p->items[i].anim);
            p->items[i].anim.flags |= 0x10;
        } else {
            func_02040ef0(&p->items[i].anim, res, tbl->unsel + k);
            func_02040e80(&p->items[i].anim);
            p->items[i].anim.flags |= 0x10;
        }
        k++;
    }
    cfg->unk0 = 7;
}
#pragma thumb reset
