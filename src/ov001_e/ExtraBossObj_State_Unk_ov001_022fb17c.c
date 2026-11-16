#include <nitro/types.h>

typedef struct UnkStruct_022fb17c_o {
    u8 pad0[0x1c];
    s32 y;
} UnkStruct_022fb17c_o;

typedef struct UnkStruct_022fb17c {
    u8 pad0[0x1c];
    s32 y;
    u8 pad20[0x54 - 0x20];
    s32 sy;
    u8 pad58[0x74 - 0x58];
    void (*state)(struct UnkStruct_022fb17c *p);
    u8 pad78[0x80 - 0x78];
    UnkStruct_022fb17c_o *target;
    u8 pad84[0x1cc - 0x84];
    s32 timer;
    s16 unk1D0;
} UnkStruct_022fb17c;

typedef struct UnkStruct_022c4560 {
    u8 pad0[0x10];
    s32 mode;
    s32 level;
} UnkStruct_022c4560;

typedef struct UnkStruct_022fb17c_c {
    u8 pad0[0x40];
    s32 chr;
} UnkStruct_022fb17c_c;

extern UnkStruct_022c4560 gGameState;
extern s32 data_ov001_0231d5ec[][5];

UnkStruct_022fb17c_c *ExtraBoss_GetWork(void);
void ExtraBossObj_State_Unk_ov001_022fb074(UnkStruct_022fb17c *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

static inline s32 GetLevel(void) {
    BOOL b = TRUE;
    if (gGameState.mode != 2 && gGameState.mode != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.level;
}

void ExtraBossObj_State_Unk_ov001_022fb17c(UnkStruct_022fb17c *p) {
    UnkStruct_022fb17c_o *o;

    p->timer--;
    if (p->timer > 0)
        return;
    o = p->target;
    p->state = ExtraBossObj_State_Unk_ov001_022fb074;
    if (o->y < p->y)
        p->sy = -data_ov001_0231d5ec[GetLevel()][ExtraBoss_GetWork()->chr];
    else
        p->sy = data_ov001_0231d5ec[GetLevel()][ExtraBoss_GetWork()->chr];
    p->unk1D0 = 0;
    Sound_PlaySeqArc(0, 0, 0xaf);
}
