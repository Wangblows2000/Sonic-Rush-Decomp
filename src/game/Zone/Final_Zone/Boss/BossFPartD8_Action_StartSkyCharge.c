#include <nitro/types.h>

typedef struct UnkStruct_022f0f44_2 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022f0f44_2;

typedef struct UnkStruct_022f0f44 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(void);
    u8 pad2[0x44e - 0x6c];
    u16 unk44E;
} UnkStruct_022f0f44;

typedef struct UnkStruct_022f0f44_3 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u32 unk18;
} UnkStruct_022f0f44_3;

extern UnkStruct_022f0f44_2 *gBossF;
extern UnkStruct_022f0f44_3 gGameState;
extern u16 data_ov000_02363dd8[];
extern s32 gBossStatus[];

void BossFPartD8_State_SkyChargeStartWait(void);

void BossFPartD8_Action_StartSkyCharge(UnkStruct_022f0f44 *s) {
    UnkStruct_022f0f44_2 *p = gBossF;
    BOOL b;
    s32 i;
    s->unk2E = 0;
    b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = gGameState.unk14;
    s->unk44E = data_ov000_02363dd8[i];
    gGameState.unk18 &= ~1;
    p->unk20C |= 0x10;
    gBossStatus[1] = 0x38;
    s->func = BossFPartD8_State_SkyChargeStartWait;
}
