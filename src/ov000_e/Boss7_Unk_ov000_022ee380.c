#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x30 - 0xC];
    u16 unk30;
    u8 pad32[0x38 - 0x32];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 pad48[0x6C - 0x48];
    u16 unk6C;
    u8 pad6E[0x74 - 0x6E];
    s32 unk74;
} UnkStruct_Cam;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 unk14;
    u8 pad18[0xDC - 0x18];
    s32 unkDC;
} UnkStruct_0236a444;

extern UnkStruct_0236a444 data_ov000_0236a444;
extern UnkStruct_Cam data_ov000_0236a48c;

s32 FX_Div(s32 a, s32 b);

void Boss7_Unk_ov000_022ee380(void) {
    UnkStruct_Cam *cam;

    *(vu16 *)0x4000304 |= 0x8000;
    data_ov000_0236a444.unkDC = 1;
    data_ov000_0236a444.unkC = 0x1a66000;
    cam = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;
    data_ov000_0236a444.unk14 = FX_Div(0x1000, 0x34cc);
    cam->unk0 = cam->unk4 = cam->unk8 = 0;
    cam->unk30 = 0x200;
    cam->unk38 = 0x400;
    cam->unk3C = cam->unk40 = cam->unk44 = 0;
    cam->unk6C = 0x400;
    cam->unk74 = 0x400;
}
