#include <nitro/types.h>

typedef struct {
    u8 pad0[0x30];
    s16 unk30;
    u8 pad32[0x38 - 0x32];
    s32 unk38;
    void *unk3C;
    void *unk40;
    u8 pad44[0x48 - 0x44];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x6c - 0x54];
    s16 unk6C;
    u8 pad6E[0x74 - 0x6e];
    s32 unk74;
    s32 unk78;
    u8 pad7C[0x84 - 0x7c];
    s32 unk84;
    u8 pad88[0x8e - 0x88];
    u16 unk8E;
} UnkStruct_0236a48c;

extern void *gPlayers;
extern UnkStruct_0236a48c data_ov000_0236a48c;

void BossFPartD8_SetCameraFollowPlayer(void) {
    UnkStruct_0236a48c *g = (UnkStruct_0236a48c *)(u32)&data_ov000_0236a48c;

    g->unk3C = gPlayers;
    g->unk40 = gPlayers;
    g->unk48 = 0x14000;
    g->unk4C = 0x32000;
    g->unk50 = 0x46000;
    g->unk6C = 0x400;
    g->unk74 = 0x2aa;
    g->unk30 = 0x400;
    g->unk38 = 0x2aa;
    g->unk78 = 0x12c000;
    g->unk84 = 0x28000;
    g->unk8E = 0xd555;
}
