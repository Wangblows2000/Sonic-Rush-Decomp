#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x224 - 0x20C];
    s32 unk224;
    s16 unk228;
    s16 unk22A;
    s16 unk22C;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x42E - 0x20C];
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u8 pad466[2];
    s16 unk468;
} UnkStruct_022e7f60;

extern UnkStruct_Boss *data_ov000_02369b78;
extern UnkStruct_StageObj *gPlayers;
extern s16 data_ov000_02363c24[][4];
extern s16 data_ov000_02363c64[][4];

void func_ov000_022e822c(void);

void func_ov000_022e7f60(UnkStruct_022e7f60 *w) {
    UnkStruct_Boss *b = data_ov000_02369b78;
    s32 d;

    b->unk22C = -8;
    if (w->base.unk2E > 0)
        d = gPlayers->unk10 - w->base.unk10;
    else
        d = w->base.unk10 - gPlayers->unk10;
    if (d > 0x800)
        return;
    w->base.unk2E = 0;
    w->unk468 >>= 1;
    b->unk224 = w->base.unk10;
    b->unk228 = data_ov000_02363c24[w->unk42E][w->unk464 - 1];
    b->unk22C = data_ov000_02363c64[w->unk42E][w->unk464 - 1] >> 1;
    b->unk22A >>= 2;
    w->unk440 = func_ov000_022e822c;
}
