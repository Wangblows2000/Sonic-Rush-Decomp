#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x43C - 0x20C];
    s32 unk43C;
    u16 unk440;
    u16 unk442;
    u8 pad444[0x458 - 0x444];
    s32 unk458;
} UnkStruct_022f168c;


extern UnkStruct_022c4560 gGameState;
extern UnkStruct_StageObj *gPlayers;
extern u16 data_ov000_02365e20[][0x18];
extern s32 data_ov000_02365e60[][3][8];
extern s16 data_ov000_02365da0[][3][8];

void BossFPartD8_SetAnimation(UnkStruct_022f168c *w, s32 a, s32 b, s32 c);
void *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void BossFPartD8_InitPartDCPath(void *o, s32 a, s32 b, s32 c);
void BossFPartD8_State_PartDCVolleySetInterval(void);

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

void BossFPartD8_State_PartDCVolleyFire(UnkStruct_022f168c *w) {
    u16 p;
    void *o;
    s32 x, y, wd;

    BossFPartD8_SetAnimation(w, 0x27, 0x1000, 1);
    w->unk442 = 0;
    p = 0;
    if (w->unk458 == 2) {
        p |= 1;
        p |= data_ov000_02365e20[GetLevel()][0] << 1;
    }
    o = MapObject_Spawn(0xdc, 0, 0, p, 0, 0, 0, 0, 0);
    wd = data_ov000_02365e60[GetLevel()][w->unk458][w->unk440];
    x = gPlayers->unk10 - 0x2000 + ((Rand() & 0x3f) << 8);
    if (w->unk43C != 0)
        x += wd >> 1;
    else
        x -= wd >> 1;
    if (w->unk43C != 0)
        y = x - wd;
    else
        y = x + wd;
    if (x < 0x1800)
        x = 0x1800;
    else if (x > 0x32800)
        x = 0x32800;
    if (y < 0x1800)
        y = 0x1800;
    else if (y > 0x32800)
        y = 0x32800;
    BossFPartD8_InitPartDCPath(o, x, y, data_ov000_02365da0[GetLevel()][w->unk458][w->unk440]);
    w->base.unk68 = BossFPartD8_State_PartDCVolleySetInterval;
}
