#include <ov000_c/stageobj.h>

typedef struct {
    s32 unk0;
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
extern UnkStruct_Player *gPlayers;

typedef struct {
    u8 pad0[0xD5E];
    u16 unkD5E;
} UnkStruct_022dffbc;

void Boss6_Unk_ov000_022dffbc(UnkStruct_022dffbc *s, s32 a) {
    UnkStruct_Cam *cam = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;

    cam->unk3C = gPlayers;
    cam->unk40 = (s32)gPlayers;
    cam->unk44 = gPlayers;
    cam->unk84 = -0x14000;
    cam->unk8C = 0x100;
    cam->unk78 = 0xa0000;
    cam->unk80 = 0x100;
    if (a != 0)
        cam->unk8E = 0x1c71;
    else
        cam->unk8E = 0xe38f;
    if (s->unkD5E != 0)
        cam->unk8E += 0x8000;
    cam->unk92 = 0x200;
}
