#include <nitro/types.h>

typedef struct UnkStruct_022cd1b0 {
    u8 pad[0x1424];
    u16 *unk1424;
    u16 *unk1428;
    u16 *unk142C;
    u16 *unk1430;
    u16 *unk1434;
    u16 *unk1438;
    u16 *unk143C;
    u16 *unk1440;
} UnkStruct_022cd1b0;

typedef struct UnkStruct_022cd1b0_2 {
    u8 pad[0x1c];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} UnkStruct_022cd1b0_2;

extern UnkStruct_022cd1b0 *gBoss2;
extern UnkStruct_022cd1b0_2 gBossStage2Models;
extern u32 data_ov000_02365110[];
extern u32 data_ov000_023651c8[];
extern u32 data_ov000_023650fc[];
extern u32 data_ov000_02365108[];
extern u16 data_ov000_02362e04[];
extern u16 data_ov000_02362e0c[];
extern u16 data_ov000_02362dc8[];
extern u16 data_ov000_02362dc4[];

void Boss_LoadModelPalettes(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void Boss2_SetPalettes(u32 flags) {
    if (flags & 3)
        return;
    if (flags & 4) {
        Boss_LoadModelPalettes(gBossStage2Models.unk1C, data_ov000_02365110, gBoss2->unk1424, data_ov000_02362e04, 3);
        Boss_LoadModelPalettes(gBossStage2Models.unk24, data_ov000_023651c8, gBoss2->unk142C, data_ov000_02362e0c, 4);
        Boss_LoadModelPalettes(gBossStage2Models.unk20, data_ov000_023650fc, gBoss2->unk1434, data_ov000_02362dc8, 1);
        Boss_LoadModelPalettes(gBossStage2Models.unk28, data_ov000_02365108, gBoss2->unk143C, data_ov000_02362dc4, 1);
    } else {
        Boss_LoadModelPalettes(gBossStage2Models.unk1C, data_ov000_02365110, gBoss2->unk1428, data_ov000_02362e04, 3);
        Boss_LoadModelPalettes(gBossStage2Models.unk24, data_ov000_023651c8, gBoss2->unk1430, data_ov000_02362e0c, 4);
        Boss_LoadModelPalettes(gBossStage2Models.unk20, data_ov000_023650fc, gBoss2->unk1438, data_ov000_02362dc8, 1);
        Boss_LoadModelPalettes(gBossStage2Models.unk28, data_ov000_02365108, gBoss2->unk1440, data_ov000_02362dc4, 1);
    }
}
