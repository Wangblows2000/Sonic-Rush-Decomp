#include <nitro/types.h>

typedef struct UnkStruct_0230b9f4 {
    u8 pad[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad1[0x8];
    s32 unk18;
    u8 pad2[0x12];
    u16 unk2E;
    u16 unk30;
    u8 pad3[0x36];
    void (*func)(void);
    u8 pad4[0x188];
    u32 unk1F4;
} UnkStruct_0230b9f4;

void Hand2d_State_Unk_ov000_0230b6a0(void);

void Hand2d_Action_Unk_ov000_0230b9f4(UnkStruct_0230b9f4 *s) {
    s->func = Hand2d_State_Unk_ov000_0230b6a0;
    s->unkC = 0;
    s->unk8 = 0;
    s->unk2E = 0;
    s->unk30 = 0;
    s->unk18 = 0;
    s->unk1F4 &= ~7;
}
