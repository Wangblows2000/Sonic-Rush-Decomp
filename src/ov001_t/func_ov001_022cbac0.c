#include <nitro/types.h>

typedef struct {
    u8 data[0x2c];
} UnkAnim_022cbac0;

typedef struct {
    u32 type;
    UnkAnim_022cbac0 anim;
} UnkItem_022cbac0;

typedef struct {
    s32 unk0;
    u8 pad4[0x2c];
    s32 tbl[(0x1e2c - 0x30) / 4];
    s32 res;
} UnkCfg_022cbac0;

typedef struct {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0xc];
    s32 unk14;
    u8 pad18[0x208 - 0x18];
    UnkAnim_022cbac0 animB;
    UnkAnim_022cbac0 animA;
    UnkAnim_022cbac0 animC;
    UnkItem_022cbac0 items[6];
    UnkCfg_022cbac0 *cfg;
} UnkStruct_022cbac0;

void func_02040ef0(void *p, s32 a, s32 b);
void func_02040e48(void *p);

#pragma thumb on
void func_ov001_022cbac0(UnkStruct_022cbac0 *p, s32 a, s32 alt) {
    UnkCfg_022cbac0 *cfg = p->cfg;
    s32 res = cfg->res;
    s32 *tbl = cfg->tbl;
    s32 i;

    func_02040ef0(&p->animA, res, tbl[14]);
    func_02040ef0(&p->animB, res, tbl[12]);
    func_02040ef0(&p->animC, res, tbl[16]);
    for (i = 0; i < 6; i++) {
        if (p->items[i].type == 7) continue;
        if (i == p->cur) {
            func_02040ef0(&p->items[i].anim, res, tbl[4]);

        } else if (i == alt) {
            func_02040ef0(&p->items[i].anim, res, tbl[6]);
        } else {
            func_02040ef0(&p->items[i].anim, res, tbl[5]);
        }
    }
    cfg->unk0 = 3;

}
#pragma thumb reset
