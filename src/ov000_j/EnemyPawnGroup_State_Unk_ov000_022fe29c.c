#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_022fe29c_1e0;

typedef struct {
    u8 pad0[0x34];
    s16 unk34;
    u8 pad36[0x5c - 0x36];
    u16 unk5C;
    u8 pad5E[0x1e0 - 0x5e];
    UnkStruct_022fe29c_1e0 *unk1E0;
} UnkStruct_022fe29c;

s16 ObjSpd_DownSet(s32 a, s32 b);
void EnemyPawnGroup_Action_Unk_ov000_022fe7e8(UnkStruct_022fe29c *p);

void EnemyPawnGroup_State_Unk_ov000_022fe29c(UnkStruct_022fe29c *p) {
    s16 spd;
    u16 f;

    spd = 0x100;
    f = p->unk1E0->unk4;
    if (f & 3)
        spd += ((f & 0xc) >> 2) << 7;
    p->unk34 = ObjSpd_DownSet(p->unk34, (spd << 6) >> 8);
    if (!(p->unk5C & 8))
        return;
    p->unk5C ^= 1;
    EnemyPawnGroup_Action_Unk_ov000_022fe7e8(p);
}
