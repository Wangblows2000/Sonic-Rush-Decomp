#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 unk2;
    u8 unk3;
    u8 pad4[0x54 - 4];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x388 - 0x334];
    u8 unk388[0x3C4 - 0x388];
    s16 unk3C4;
    u8 pad3C6[0x3CF - 0x3C6];
    u8 unk3CF;
    u8 pad3D0[0x3E8 - 0x3D0];
    void *unk3E8;
    s16 unk3EC;
    s16 unk3EE;
    s32 unk3F0;
    s32 unk3F4;
    s32 unk3F8;
    s32 unk3FC;
    u8 pad400[0x410 - 0x400];
    u8 unk410;
} UnkStruct_0234c9e8;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_0234c9e8_4560;

extern UnkStruct_0234c9e8_4560 gGameState;

void NNS_SndPlayerStopSeq(void *p, s32 a);

void Player_ResetGimmickState(UnkStruct_0234c9e8 *w) {
    w->unk3E8 = NULL;
    w->unk3EC = 0;
    w->unk3EE = 0;
    w->unk3F0 = 0;
    w->unk3F4 = 0;
    w->unk3F8 = 0;
    w->unk3FC = 0;
    w->unk3CF = 0;
    w->unk3C4 = 0;
    if (w->unk410 & 0x80) {
        w->unk54 |= 1;
        w->unk410 = 0;
    }
    w->unk32C &= ~0x80000;
    w->unk32C &= ~0xf;
    w->unk32C &= ~0xc000;
    w->unk330 &= ~0x40000;
    w->unk330 &= 0xfffdff0f;
    w->unk32C &= 0xffefdfff;
    w->unk5C |= 0x100;
    w->unk58 |= 0x80;
    w->unk58 |= 0x40;
    w->unk58 &= ~0x2000;
    w->unk58 &= ~0x100;
    w->unk58 &= ~0x200;
    w->unk58 &= ~0x8000;
    w->unk58 &= ~0x4000;
    w->unk58 |= 0x40000;
    w->unk58 |= 0x20000;
    w->unk58 |= 0x200000;
    w->unk58 &= ~0x10000000;
    w->unk5C &= ~0x20;
    w->unk3 = 0;
    w->unk2 = 0;
    if (gGameState.unk1C < 9 && gGameState.unk1D < 2) {
        w->unk5C |= 0x200;
    }
    NNS_SndPlayerStopSeq(w->unk388, 0);
}
