#include <ov000_b/stageobj.h>

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x270 - 4];
    s16 unk270;
    s16 unk272;
    u16 unk274;
    u16 unk276;
    s32 unk278[5];
    u16 unk28C;
    u16 unk28E;
    u16 unk290;
    s16 unk292;
    s16 unk294;
} UnkStruct_023367ac_S;

typedef struct {
    u8 pad0[0x688];
    UnkStruct_023367ac_S unk688;
} UnkStruct_023367ac;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_023367ac_Sys;

extern UnkStruct_023367ac_Sys gGameState;

u16 FX_DivS32(s32 a, s32 b);

void Hud_Unk_ov000_023367ac(UnkStruct_023367ac *w) {
    UnkStruct_023367ac_S *s = &w->unk688;
    s32 sh;
    s32 i;

    s->unk292 = s->unk294 = 0;
    if (s->unk290 != 0) {
        s->unk290--;
        if (gGameState.unk20 & 1) {
            sh = (40 - s->unk290) >> 4;
            s->unk292 = (4 - (Rand() & 7)) >> sh;
            s->unk294 = (4 - (Rand() & 7)) >> sh;
        }
    }
    if (s->unk270 < s->unk272) {
        if (s->unk274 != 0)
            s->unk274--;
        if (s->unk274 == 0) {
            s->unk272--;
            if (s->unk272 < s->unk270)
                s->unk272 = s->unk270;
            s->unk274 = 1;
        }
    } else {
        if (s->unk270 > s->unk272) {
            if (s->unk276 != 0)
                s->unk276--;
            if (s->unk276 == 0) {
                s->unk272 += 2;
                if (s->unk272 > s->unk270)
                    s->unk272 = s->unk270;
            }
            s->unk28E = s->unk28C;
            s->unk28C = FX_DivS32(s->unk272, 5);
            if (s->unk28E != s->unk28C) {
                for (i = 2; i >= 0; i--)
                    s->unk278[i + 1] = s->unk278[i];
                s->unk278[0] = s->unk278[4];
            }
        }
        for (i = 0; i < 4; i++) {
            if (s->unk278[i] > 0x1000) {
                s->unk278[i] -= 0x333;
                if (s->unk278[i] < 0x1000)
                    s->unk278[i] = 0x1000;
            }
        }
    }
    s->unk0 = s->unk292;
    s->unk2 = s->unk294;
}
