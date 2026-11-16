#include <nitro/types.h>

typedef struct UnkStruct_022d836c {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x38];
    void (*func)(void);
    u8 pad2[0x1ee];
    u16 unk25A;
    u8 pad3[0x174];
    u8 unk3D0;
} UnkStruct_022d836c;

void Boss3PartCD_State_Unk_ov000_022d81ac(void);

void Boss3PartCD_State_Unk_ov000_022d836c(UnkStruct_022d836c *s) {
    s->unk3D0 |= 1;
    s->unk25A = 0;
    s->unk2E = 0;
    s->func = Boss3PartCD_State_Unk_ov000_022d81ac;
}
