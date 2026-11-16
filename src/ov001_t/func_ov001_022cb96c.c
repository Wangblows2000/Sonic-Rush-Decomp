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

void func_02040ef0(void *p, s32 a, s32 b);
void func_02040e48(void *p);

#pragma thumb on
void func_ov001_022cb96c(UnkStruct_022cb96c *p) {
    UnkCfg_022cb96c *cfg = p->cfg;
    s32 res = cfg->res;
    UnkTbl_022cb96c *tbl = &cfg->tbl;
    s32 i;

    func_02040ef0(&p->animA, res, tbl->animA);
    func_02040ef0(&p->animB, res, tbl->animB);
    func_02040ef0(&p->animC, res, tbl->animC);
    for (i = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (i == p->cur) {
            func_02040ef0(&p->items[i].anim, res, tbl->sel);
            func_02040e48(&p->items[i].anim);
        } else {
            func_02040ef0(&p->items[i].anim, res, tbl->unsel);
        }
    }
    cfg->unk0 = 6;
    p->unk14 = 0;
}
#pragma thumb reset
