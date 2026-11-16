#include <nitro/types.h>

typedef struct {
    u8 pad0[0x43c];
    void *unk43C;
    u8 pad440[0x454 - 0x440];
    u16 unk454;
    s16 unk456;
} UnkStruct_022ea20c;

void Boss7PartD5_InputState_Unk_ov000_022ea1b8(UnkStruct_022ea20c *p);

u16 Boss7PartD5_InputState_Unk_ov000_022ea20c(UnkStruct_022ea20c *p) {
    u16 r = 0x80;

    if (p->unk456 > 0)
        r |= 0x10;
    else
        r |= 0x20;
    if (p->unk454++ > 4)
        p->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea1b8;
    return r;
}
