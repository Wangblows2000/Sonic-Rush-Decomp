#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3c];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 pad48[0x58 - 0x48];
    s32 unk58;
    u8 pad5C[0x6c - 0x5c];
    u16 unk6C;
    u8 pad6E[0x78 - 0x6e];
    s32 unk78;
    u8 pad7C[0x80 - 0x7c];
    u16 unk80;
    u8 pad82[0x84 - 0x82];
    s32 unk84;
    u8 pad88[0x8c - 0x88];
    u16 unk8C;
} UnkStruct_0236a48c;

extern s32 gPlayers;
extern UnkStruct_0236a48c data_ov000_0236a48c;

void Boss6_SetCameraJumpStart(void) {
    UnkStruct_0236a48c *g = (UnkStruct_0236a48c *)(u32)&data_ov000_0236a48c;

    g->unk3C = gPlayers;
    g->unk40 = 0;
    g->unk44 = gPlayers;
    g->unk58 = -0xdc000;
    g->unk6C = 0x100;
    g->unk84 = -0x15e000;
    g->unk8C = 0x100;
    g->unk78 = 0x258000;
    g->unk80 = 0x100;
}
