#include <nitro/types.h>

typedef struct UnkStruct_02305450_2 {
    u8 pad[6];
    s8 unk6;
} UnkStruct_02305450_2;

typedef struct UnkStruct_02305450 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x94 - 0x18];
    u8 unk94[0xdc - 0x94];
    u8 unkDC[0x128 - 0xdc];
    s32 unk128;
    u8 pad2[0x1e0 - 0x12c];
    UnkStruct_02305450_2 *unk1E0;
} UnkStruct_02305450;

typedef struct UnkStruct_02305450_3 {
    u8 pad[0x10];
    UnkStruct_02305450 *unk10;
} UnkStruct_02305450_3;

extern UnkStruct_02305450_3 *gTaskCurrent;

void StageObj_Draw3D(UnkStruct_02305450 *s, s32 a);
void StageObj_Draw2D(UnkStruct_02305450 *s, void *a, void *b);

void CranePartEE_Draw(void) {
    UnkStruct_02305450 *p = gTaskCurrent->unk10;
    s32 x = p->unk10;
    s32 y = p->unk14;
    s16 i;
    for (i = 0; i < p->unk1E0->unk6 + 1; i++) {
        StageObj_Draw3D(p, p->unk128);
        StageObj_Draw2D(p, p->unk94, p->unkDC);
        p->unk14 += 0x4000;
    }
    p->unk14 = y;
    p->unk10 = x;
}
