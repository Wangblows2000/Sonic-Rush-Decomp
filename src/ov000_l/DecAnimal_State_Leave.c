#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_0231d79c_A;

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x184 - 0x134];
    UnkStruct_0231d79c_A *unk184;
} UnkWork_0231d79c;

s16 ObjSpd_UpSet(s32 v, s32 a, s32 b);
void DecAnimal_UpdateMove(UnkWork_0231d79c *w);

void DecAnimal_State_Leave(UnkWork_0231d79c *w) {
    switch (w->unk184->unk2) {
    case 5:
        if (w->base.unk5C & 8)
            w->base.unk54 |= 4;
        break;
    case 6:
    case 12:
        w->base.unk58 &= ~0x80;
        w->base.unk58 |= 0x110;
        w->base.unk30 = ObjSpd_UpSet(w->base.unk30, -16, 0x200);
        break;
    }
    DecAnimal_UpdateMove(w);
}
