#include <nitro/types.h>

typedef struct UnkStruct_022d5b38 {
    u8 pad[0x13e];
    u16 unk13E;
    u8 pad1[0x16a - 0x140];
    u16 unk16A;
    u8 pad2[0x20c - 0x16c];
    u32 unk20C;
} UnkStruct_022d5b38;

void Boss3PartCD_Unk_ov000_022d5d14(UnkStruct_022d5b38 *s);

void Boss3PartCD_State_Unk_ov000_022d5b38(UnkStruct_022d5b38 *s) {
    s->unk13E &= ~4;
    s->unk16A |= 4;
    s->unk20C |= 0x10;
    Boss3PartCD_Unk_ov000_022d5d14(s);
}
