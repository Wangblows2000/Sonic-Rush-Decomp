#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s16 unk48;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4590;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3AE - 0x20C];
    s16 unk3AE;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0xF50 - 0x20C];
    s32 unkF50;
} UnkStruct_022e20b0;

extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_Player *gPlayers;

void func_0204eb04(void);
void func_0204d5f8(void (*fn)(void));

void Boss6_State_Unk_ov000_022e20b0(UnkStruct_022e20b0 *w) {
    w->unkF50--;
    if ((w->unkF50 & 3) == 0) {
        if (gDisplayMain.unk48 > 0)
            gDisplayMain.unk48--;
    }
    if (w->unkF50 > 0)
        return;
    data_022c4590.unk10 = gPlayers->unk3AE * 1000;
    func_0204d5f8(func_0204eb04);
}
