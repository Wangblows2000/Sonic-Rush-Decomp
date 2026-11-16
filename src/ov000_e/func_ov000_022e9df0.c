#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022e9df0 {
    u8 pad0[0x42E];
    u16 unk42E;
    u8 pad430[0x43C - 0x430];
    void (*unk43C)(struct UnkStruct_022e9df0 *);
} UnkStruct_022e9df0;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;
extern u8 data_ov000_02363b4c[];
extern u8 data_ov000_02363b1c[];
extern u8 data_ov000_02363b2c[];

void Boss7PartD5_InputState_Unk_ov000_022ea798(UnkStruct_022e9df0 *w);

s32 func_ov000_022e9df0(UnkStruct_022e9df0 *w) {
    if (gBossStatus.unk8 >= 6) {
        w->unk42E = data_ov000_02363b4c[Rand() & 7];
    } else if (gBossStatus.unk8 >= 4) {
        w->unk42E = data_ov000_02363b1c[Rand() & 7];
    } else {
        w->unk42E = data_ov000_02363b2c[Rand() & 7];
    }
    w->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea798;
    return 4;
}
