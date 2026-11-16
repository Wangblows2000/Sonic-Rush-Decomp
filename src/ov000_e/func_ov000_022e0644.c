#include <ov000_c/stageobj.h>

typedef struct {
    void *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x30 - 0x24];
    u16 unk30;
    u8 pad32[0x3C - 0x32];
    void *unk3C;
    s32 unk40;
    void *unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[0x6C - 0x60];
    u16 unk6C;
    u8 pad6E[0x78 - 0x6E];
    s32 unk78;
    s32 unk7C;
    u16 unk80;
    u8 pad82[0x84 - 0x82];
    s32 unk84;
    s32 unk88;
    u16 unk8C;
    u16 unk8E;
    u16 unk90;
    u16 unk92;
} UnkStruct_Cam;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x14 - 0x10];
    s32 unk14;
    u8 pad18[0xDC - 0x18];
    s32 unkDC;
} UnkStruct_0236a444;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x42C - 0x20C];
    u16 unk42C;
    u16 unk42E;
    u16 unk430;
    u16 unk432;
} UnkStruct_Player;

extern UnkStruct_0236a444 data_ov000_0236a444;
extern UnkStruct_Cam data_ov000_0236a48c;
extern UnkStruct_Player *data_ov000_0236ac60;

typedef struct {
    u8 pad0[0xD5E];
    u16 unkD5E;
} UnkStruct_022e0644;

void func_ov000_022e0644(UnkStruct_022e0644 *s) {
    UnkStruct_Cam *cam;

    data_ov000_0236a444.unkDC = 1;
    cam = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;
    cam->unk0 = data_ov000_0236ac60;
    cam->unk1C = -0x1f0000;
    cam->unk30 = 0x200;
    cam->unk3C = (void *)(cam->unk40 = (s32)(cam->unk44 = 0));
    cam->unk3C = s;
    cam->unk58 = -0x19c000;
    cam->unk6C = 0x200;
    if (s->unkD5E != 0) {
        cam->unk20 = -0x64000;
        cam->unk5C = 0xc8000;
    } else {
        cam->unk20 = 0x64000;
        cam->unk5C = -0xc8000;
    }
}
