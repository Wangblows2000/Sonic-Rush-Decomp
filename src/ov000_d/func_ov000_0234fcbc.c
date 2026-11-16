#include <nitro/types.h>

typedef struct UnkStruct_0234fcbc {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x2E - 2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32C - 0x6C];
    u32 unk32C;
    u8 pad330[0x340 - 0x330];
    void (*unk340)(struct UnkStruct_0234fcbc *w);
    u8 pad344[0x384 - 0x344];
    u8 unk384[0x3E8 - 0x384];
    void *unk3E8;
} UnkStruct_0234fcbc;

void Player_State_Pipe(void);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void func_ov000_0234fcbc(UnkStruct_0234fcbc *w, s16 spd) {
    u8 dir;

    if (w->unk68 != Player_State_Pipe) {
        return;
    }
    dir = ((w->unk1 - 0x20) & 0xc0) >> 6;
    if (w->unk32C & 1) {
        dir = (dir + 2) & 3;
    }
    if (spd == 0) {
        s32 t = w->unk34;
        if (t < 0) {
            t = -t;
        }
        spd = t;
    }
    w->unk3E8 = NULL;
    w->unk58 &= ~0x2000;
    w->unk58 |= 0x80;
    w->unk58 |= 0x40000;
    w->unk58 |= 0x20000;
    w->unk58 &= ~0x200;
    w->unk1 = 0;
    w->unk58 &= ~0x100;
    w->unk58 &= ~0x4000;
    w->unk32C &= ~0x100000;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x40);
    w->unk34 = 0;
    w->unk340(w);
    switch (dir) {
    case 0:
    default:
        w->unk30 = spd;
        break;
    case 1:
        w->unk5C |= 1;
        w->unk2E = -spd;
        w->unk30 = 0;
        break;
    case 2:
        w->unk30 = -spd;
        break;
    case 3:
        w->unk2E = spd;
        w->unk30 = 0;
        break;
    }
    w->unk32C |= 1;
}
