#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u32 flags;
    u8 pad14[0x18];
} UnkAnim_022cbc2c;

typedef struct {
    u32 type;
    UnkAnim_022cbc2c anim;
} UnkItem_022cbc2c;

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
} UnkTbl_022cbc2c;

typedef struct {
    s32 unk0;
    u8 pad4[0x2c];
    UnkTbl_022cbc2c tbl;
    u8 pad[0x1e2c - 0x30 - sizeof(UnkTbl_022cbc2c)];
    s32 res;
} UnkCfg_022cbc2c;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0x208 - 8];
    UnkAnim_022cbc2c animB;
    UnkAnim_022cbc2c animA;
    UnkAnim_022cbc2c animC;
    UnkItem_022cbc2c items[6];
    UnkCfg_022cbc2c *cfg;
} UnkStruct_022cbc2c;

void func_02040ef0(void *p, s32 a, s32 b);
void func_02040e80(void *p);

#pragma thumb on
void func_ov001_022cbc2c(UnkStruct_022cbc2c *p) {
    UnkCfg_022cbc2c *cfg = p->cfg;
    s32 res = cfg->res;
    UnkTbl_022cbc2c *tbl = &cfg->tbl;
    s32 k;
    s32 i;

    func_02040ef0(&p->animA, res, tbl->animA);
    func_02040ef0(&p->animB, res, tbl->animB);
    func_02040ef0(&p->animC, res, tbl->animC);
    for (i = 0, k = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (p->cur == i) {
            func_02040ef0(&p->items[i].anim, res, tbl->sel + k);
        } else {
            func_02040ef0(&p->items[i].anim, res, tbl->unsel + k);
        }
        k++;
    }
    cfg->unk0 = 1;
    p->cur = 0;
}
#pragma thumb reset
