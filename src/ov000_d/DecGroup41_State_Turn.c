#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    u8 pad30[0x5C - 0x30];
    u16 unk5C;
    u8 pad5E[0x9C - 0x5E];
    u16 unk9C;
} UnkStruct_02321514;

extern u32 gRandSeed;

s16 ObjSpd_DownSet(s16 v, s32 a);
void StageObj_SetAnimation(void *w, u16 a);
void DecGroup41_Action_Sink(UnkStruct_02321514 *w);
void DecGroup41_Action_Dart(UnkStruct_02321514 *w, s16 x, s16 y, s32 a);

static inline u16 UnkRand_02321514(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void DecGroup41_State_Turn(UnkStruct_02321514 *w) {
    w->unk2E = ObjSpd_DownSet(w->unk2E, 8);
    if (!(w->unk5C & 8)) {
        return;
    }
    StageObj_SetAnimation(w, w->unk9C - 1);
    w->unk5C ^= 1;
    if (!(UnkRand_02321514() & 3)) {
        DecGroup41_Action_Sink(w);
        return;
    }
    if (w->unk5C & 1) {
        DecGroup41_Action_Dart(w, -0x81 - (UnkRand_02321514() & 0xfe), 0xff - (UnkRand_02321514() & 0x1fe), UnkRand_02321514() & 7);
    } else {
        DecGroup41_Action_Dart(w, 0x17f - (UnkRand_02321514() & 0xfe), 0xff - (UnkRand_02321514() & 0x1fe), UnkRand_02321514() & 7);
    }
}
