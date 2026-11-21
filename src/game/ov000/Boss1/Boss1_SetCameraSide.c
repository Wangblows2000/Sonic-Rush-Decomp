#include <nitro/types.h>

typedef struct UnkStruct_022c71d8 {
    u8 pad[0x90];
    s32 unk90;
    s32 unk94;
    s32 unk98;
    u8 pad1[0xc0 - 0x9c];
    s32 unkC0;
    u8 pad2[0xcc - 0xc4];
    s32 unkCC;
    u8 pad3[0xd6 - 0xd0];
    u16 unkD6;
} UnkStruct_022c71d8;

typedef struct UnkStruct_022c71d8_2 {
    u8 pad[0x430];
    u16 unk430;
    u16 unk432;
} UnkStruct_022c71d8_2;

extern UnkStruct_022c71d8 data_ov000_0236a444;
extern UnkStruct_022c71d8_2 *gPlayers;

void Boss1_SetCameraSide(s32 dir) {
    data_ov000_0236a444.unk90 = 0;
    data_ov000_0236a444.unk94 = 0;
    data_ov000_0236a444.unk98 = 0;
    data_ov000_0236a444.unkC0 = 0x8c000;
    data_ov000_0236a444.unkCC = 0xf000;
    if (dir > 0) {
        data_ov000_0236a444.unkD6 = 0xcaaa;
        gPlayers->unk430 = 0x10;
        gPlayers->unk432 = 0x20;
    } else {
        data_ov000_0236a444.unkD6 = 0x3555;
        gPlayers->unk430 = 0x20;
        gPlayers->unk432 = 0x10;
    }
}
