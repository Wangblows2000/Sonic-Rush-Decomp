#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_023329ec_88;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[4];
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u8 pad58[4];
    u16 unk5C;
    u8 pad5E[0x88 - 0x5E];
    UnkStruct_023329ec_88 *unk88;
} UnkStruct_023329ec;

extern s16 data_0206e470[];

void StageObj_SetOAMPriority(void *w, s32 a);

static inline s16 UnkCos_023329ec(u16 a) {
    s32 i = (s32)(a & 0xffff) >> 4;
    return data_0206e470[(u32)((i << 1) + 1)];
}

void EffBlazeSpinFlame_State_Main(UnkStruct_023329ec *w) {
    if (w->unk88 != NULL) {
        w->unk10 = w->unk88->unk10 + (w->unk28 << 8);
        w->unk14 = w->unk88->unk14 + (w->unk2A << 8);
        if (w->unk5C & 1) {
            w->unk10 -= w->unk1C;
        } else {
            w->unk10 += w->unk1C;
        }
        if (w->unk5C & 2) {
            w->unk14 -= w->unk20;
        } else {
            w->unk14 += w->unk20;
        }
        w->unkC -= 0x600;
        w->unk2E = (w->unk8 * UnkCos_023329ec(w->unkC)) >> 12;
        w->unk30 -= 10;
        w->unk8 += 0x18;
        w->unk1C += w->unk2E;
        w->unk20 += w->unk30;
        if (w->unk2E < 0) {
            StageObj_SetOAMPriority(w, 2);
        } else {
            StageObj_SetOAMPriority(w, 1);
        }
    }
    if (w->unk5C & 8) {
        w->unk54 |= 4;
    }
}
