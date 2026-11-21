#include <nitro/types.h>

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    s16 unk2A;
} UnkStruct_02321cb8_P;

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x194 - 0x6C];
    u32 unk194;
} UnkStruct_02321cb8;

void StageObj_SetAnimation(void *w, s32 a);
void DecBird_State_FlyAway(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void DecBird_Action_FlyAway(UnkStruct_02321cb8 *w, UnkStruct_02321cb8_P *p) {
    s16 s;

    StageObj_SetAnimation(w, 1);
    w->unk5C |= 4;
    w->unk68 = DecBird_State_FlyAway;
    w->unk58 &= ~0x2000;
    w->unk54 |= 2;
    w->unk2E = 0xa0;
    w->unk30 = -0x120;
    if (p != NULL) {
        s = 0;
        s += (s16)(ABS(p->unk28) + ABS(p->unk2A));
        s >>= 3;
        w->unk2E += s;
        w->unk30 -= s;
    }
    if (w->unk5C & 1)
        w->unk2E = -w->unk2E;
    w->unk194 |= 0x10;
}
