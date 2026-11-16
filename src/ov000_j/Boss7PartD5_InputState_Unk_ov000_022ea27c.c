#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5c];
    u16 unk5C;
    u8 pad5E[0x43c - 0x5e];
    void *unk43C;
    u8 pad440[0x454 - 0x440];
    u16 unk454;
} UnkStruct_022ea27c;

void Boss7PartD5_InputState_Unk_ov000_022ea798(UnkStruct_022ea27c *p);

u16 Boss7PartD5_InputState_Unk_ov000_022ea27c(UnkStruct_022ea27c *p) {
    u16 r = 0;

    if (p->unk454++ > 3) {
        if (p->unk5C & 1)
            r |= 0x10;
        else if (!(p->unk5C & 1))
            r |= 0x20;
        p->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea798;
    }
    return r;
}
