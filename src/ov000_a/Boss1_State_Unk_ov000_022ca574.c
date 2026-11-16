#include <nitro/types.h>

typedef struct UnkStruct_022ca574_3 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x334 - 0x58];
    u8 unk334;
} UnkStruct_022ca574_3;

typedef struct UnkStruct_022ca574 {
    u8 pad[0x210];
    UnkStruct_022ca574_3 *unk210;
    UnkStruct_022ca574_3 *unk214;
    u8 pad1[0x220 - 0x218];
    u16 unk220;
} UnkStruct_022ca574;

typedef struct UnkStruct_022ca574_4 {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022ca574_4;

typedef struct UnkStruct_022ca574_5 {
    u8 pad[0x3ae];
    s16 unk3AE;
} UnkStruct_022ca574_5;

typedef struct UnkStruct_022ca574_6 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022ca574_6;

extern UnkStruct_022ca574_4 gDisplayMain;
extern UnkStruct_022ca574_5 *gPlayers;
extern UnkStruct_022ca574_6 data_022c4590;

void Stage_OnBossCleared(void);
void StageShutdown_Create(void (*f)(void));

void Boss1_State_Unk_ov000_022ca574(UnkStruct_022ca574 *s) {
    UnkStruct_022ca574_3 *p;
    s->unk220++;
    if ((s->unk220 & 3) == 0) {
        if (gDisplayMain.unk48 > 0)
            gDisplayMain.unk48--;
    }
    p = s->unk210;
    if (!(p->unk334 & 2))
        return;
    if (s->unk220 <= 0x12c)
        return;
    p->unk54 |= 4;
    s->unk214->unk54 |= 4;
    data_022c4590.unk10 = gPlayers->unk3AE * 1000;
    StageShutdown_Create(Stage_OnBossCleared);
}
