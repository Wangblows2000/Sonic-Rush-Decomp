#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkStruct_022e7210;

typedef struct {
    u8 pad0[0x3ae];
    s16 unk3AE;
} UnkStruct_022e7210_plr;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4590;

extern UnkStruct_022e7210_plr *gPlayers;
extern UnkStruct_022c4590 data_022c4590;

void StageShutdown_Create(void (*f)(void));
void Stage_OnBossCleared(void);

void Boss7PartD5_MoveState_Unk_ov000_022e7210(UnkStruct_022e7210 *p) {
    if (p->unk14 <= 0x6b000)
        return;
    data_022c4590.unk10 = gPlayers->unk3AE * 1000;
    StageShutdown_Create(Stage_OnBossCleared);
}
