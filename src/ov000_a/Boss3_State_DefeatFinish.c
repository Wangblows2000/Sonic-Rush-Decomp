#include <nitro/types.h>

typedef struct UnkStruct_022d9264_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x3d0 - 0x58];
    u8 unk3D0;
} UnkStruct_022d9264_o;

typedef struct UnkStruct_022d9264 {
    u8 pad[0x214];
    UnkStruct_022d9264_o *unk214;
    UnkStruct_022d9264_o *unk218;
    u8 pad1[0x224 - 0x21c];
    u16 unk224;
} UnkStruct_022d9264;

typedef struct UnkStruct_022d9264_g {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022d9264_g;

typedef struct UnkStruct_022d9264_p {
    u8 pad[0x3ae];
    s16 unk3AE;
} UnkStruct_022d9264_p;

typedef struct UnkStruct_022d9264_c {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022d9264_c;

extern UnkStruct_022d9264_g gDisplayMain;
extern UnkStruct_022d9264_p *gPlayers;
extern UnkStruct_022d9264_c data_022c4590;

void Stage_OnBossCleared(void);
void StageShutdown_Create(void (*fn)(void));

void Boss3_State_DefeatFinish(UnkStruct_022d9264 *s) {
    UnkStruct_022d9264_o *a = s->unk214;
    UnkStruct_022d9264_o *b = s->unk218;
    s->unk224++;
    if (!(s->unk224 & 3)) {
        if (gDisplayMain.unk48 > 0)
            gDisplayMain.unk48--;
    }
    if (!(a->unk3D0 & 2))
        return;
    if (s->unk224 <= 0x12c)
        return;
    a->unk54 |= 4;
    b->unk54 |= 4;
    data_022c4590.unk10 = gPlayers->unk3AE * 1000;
    StageShutdown_Create(Stage_OnBossCleared);
}
