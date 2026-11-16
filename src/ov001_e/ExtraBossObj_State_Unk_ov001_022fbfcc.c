#include <nitro/types.h>

typedef struct UnkStruct_022fbfcc {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x50 - 0x18];
    s32 vx;
    s32 vy;
    u8 pad58[4];
    s32 unk5C;
    s32 unk60;
    u8 pad64[0x6c - 0x64];
    s16 unk6C;
    u8 pad6E[0x74 - 0x6e];
    void (*state)(struct UnkStruct_022fbfcc *p);
    u8 pad78[0xb8 - 0x78];
    u32 flagsB8;
    u8 padBC[0x1cc - 0xbc];
    s32 unk1CC;
    u8 pad1D0[4];
    s32 unk1D4;
    s32 timer;
} UnkStruct_022fbfcc;

void ExtraBossObj_State_Unk_ov001_022fbbc8(UnkStruct_022fbfcc *p);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void ExtraBossObj_State_Unk_ov001_022fbfcc(UnkStruct_022fbfcc *p) {
    s32 ax;
    s32 ay;

    p->timer--;
    if (p->timer > 0) {
        if (p->timer & 2)
            p->flags |= 0x20000;
        else
            p->flags &= ~0x20000;
    } else if (p->timer == 0) {
        p->flags &= ~0x20000;
        p->flagsB8 &= ~0x200;
    }
    ay = p->vy;
    ax = p->vx;
    if (ay < 0)
        ay = -ay;
    if (ax < 0)
        ax = -ax;
    if (ax + ay > 0x1000)
        return;
    p->state = ExtraBossObj_State_Unk_ov001_022fbbc8;
    p->unk5C = p->unk60 = 0;
    p->unk6C = 0x2d8;
    p->unk1CC = 0x78;
    p->unk1D4 = 600;
    ExtraBossObj_State_Unk_ov001_022fbbc8(p);
}
