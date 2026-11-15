#include <nitro/types.h>

typedef struct { u8 unk0[0x12]; u16 unk12; } UnkSub_02020960;
typedef struct { u8 unk0[0x24]; UnkSub_02020960 unk24; } UnkStruct_02020960;

u16 func_02020960(UnkStruct_02020960 *s, u32 v) {
    UnkSub_02020960 *sub = &s->unk24;
    u16 old = sub->unk12;
    sub->unk12 &= ~1;
    sub->unk12 |= (v & 1);
    return old & 1;
}
