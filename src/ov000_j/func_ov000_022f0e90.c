#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 a : 1;
            u16 pad1 : 5;
            u16 b : 2;
            u16 pad8 : 8;
        } bits;
    } unk20;
    u8 pad22[2];
    struct {
        u16 c : 5;
        u16 pad5 : 11;
    } unk24;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x454 - 0x6c];
    u16 unk454;
} UnkStruct_022f0e90;

extern UnkStruct_0208922c gDisplayMain;
extern u8 gBGMSoundHandle[];

void NNS_SndPlayerStopSeq(void *a, s32 b);
void BossFPartD8_State_Unk_ov000_022f0e38(UnkStruct_022f0e90 *p);

void func_ov000_022f0e90(UnkStruct_022f0e90 *p) {
    gDisplayMain.unk20.raw = 0;
    gDisplayMain.unk20.bits.a = 1;
    gDisplayMain.unk20.bits.b = 2;
    gDisplayMain.unk24.c = 0;
    NNS_SndPlayerStopSeq(gBGMSoundHandle, 0x3c);
    p->unk454 = 0;
    p->unk68 = BossFPartD8_State_Unk_ov000_022f0e38;
}
