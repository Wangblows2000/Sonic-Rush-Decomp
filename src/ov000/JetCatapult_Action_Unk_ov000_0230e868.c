#include <nitro/types.h>

typedef struct UnkStruct_0230e868 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x30 - 0x10];
    u16 unk30;
    u8 pad2[0x54 - 0x32];
    u32 unk54;
    u8 pad3[0x68 - 0x58];
    void (*func)(void);
} UnkStruct_0230e868;

void JetCatapult_State_Unk_ov000_0230e7ec(void);
void EffJet_Create(UnkStruct_0230e868 *s);

void JetCatapult_Action_Unk_ov000_0230e868(UnkStruct_0230e868 *s) {
    s->func = JetCatapult_State_Unk_ov000_0230e7ec;
    s->unk54 |= 2;
    s->unkC = 0xb6;
    s->unk30 = 0x100;
    EffJet_Create(s);
}
