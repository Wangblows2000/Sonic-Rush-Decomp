#include <nitro/types.h>

typedef struct UnkStruct_022dcd80 {
    u8 pad[0x21c];
    u32 unk21C;
    u8 pad1[0xbd0 - 0x220];
    s32 unkBD0;
    s32 unkBD4;
    s32 unkBD8;
} UnkStruct_022dcd80;

typedef struct UnkStruct_022dcd80_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022dcd80_7;

extern UnkStruct_022dcd80_7 gBossStatus;
extern u8 data_ov000_02363550[];

void Boss4_LoadModelPalettes2Warn(s32 a);
void Boss4_ActivateAuxModel(UnkStruct_022dcd80 *s);
void Boss4_DeactivateAuxModel(UnkStruct_022dcd80 *s);
BOOL Boss4_RandomBitInMask(u32 mask, s32 n);

void Boss4_UpdateAuxModelCycle(UnkStruct_022dcd80 *s) {
    s->unkBD4--;
    if (s->unkBD8 == 1) {
        if (s->unkBD4 & 6)
            Boss4_LoadModelPalettes2Warn(1);
        else
            Boss4_LoadModelPalettes2Warn(0);
    }
    if (s->unkBD4 > 0)
        return;
    if (s->unkBD8 == 1) {
        Boss4_LoadModelPalettes2Warn(0);
        Boss4_ActivateAuxModel(s);
        return;
    }
    if (s->unkBD0 == 1) {
        if (Boss4_RandomBitInMask((s->unk21C & 1) ? 0xff : data_ov000_02363550[gBossStatus.unk8 - 1], 8)) {
            Boss4_DeactivateAuxModel(s);
            return;
        }
        Boss4_ActivateAuxModel(s);
        return;
    }
    s->unkBD8 = 1;
    s->unkBD4 = 0x3c;
}
