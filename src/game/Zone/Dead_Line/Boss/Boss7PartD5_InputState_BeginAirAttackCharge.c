#include <game/Stage/stageobj.h>

typedef struct UnkStruct_022e9f8c {
    u8 pad0[0x42E];
    u16 unk42E;
    u8 pad430[0x43C - 0x430];
    void (*unk43C)(struct UnkStruct_022e9f8c *);
} UnkStruct_022e9f8c;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;
extern u8 data_ov000_02363b54[];
extern u8 data_ov000_02363b34[];
extern u8 data_ov000_02363b5c[];

void Boss7PartD5_InputState_HoldAirAttackCharge(UnkStruct_022e9f8c *w);

void Boss7PartD5_InputState_BeginAirAttackCharge(UnkStruct_022e9f8c *w) {
    if (gBossStatus.unk8 >= 6) {
        w->unk42E = data_ov000_02363b54[Rand() & 7];
    } else if (gBossStatus.unk8 >= 4) {
        w->unk42E = data_ov000_02363b34[Rand() & 7];
    } else {
        w->unk42E = data_ov000_02363b5c[Rand() & 7];
    }
    w->unk43C = Boss7PartD5_InputState_HoldAirAttackCharge;
    w->unk43C(w);
}
