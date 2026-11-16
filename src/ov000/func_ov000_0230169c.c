#include <nitro/types.h>

typedef struct UnkStruct_0230169c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x36];
    void (*func)(void);
    u8 pad2[0x1cc];
    u32 unk238;
    s32 unk23C;
} UnkStruct_0230169c;

void Kuragen_State_Unk_ov000_023014e8(void);

void func_ov000_0230169c(UnkStruct_0230169c *s) {
    s->func = Kuragen_State_Unk_ov000_023014e8;
    s->unk30 = 0;
    s->unk23C = 0;
    s->unk238 &= ~3;
}
