#include <nitro/types.h>

typedef struct UnkStruct_022ea364 {
    u8 pad0[0x5c];
    u16 unk5C;
    u8 pad5E[0x43c - 0x5e];
    void (*unk43C)(struct UnkStruct_022ea364 *p);
    u8 pad440[0x454 - 0x440];
    u16 unk454;
    s16 unk456;
} UnkStruct_022ea364;

void Boss7PartD5_InputState_Unk_ov000_022ea33c(UnkStruct_022ea364 *p);

u16 Boss7PartD5_InputState_Unk_ov000_022ea364(UnkStruct_022ea364 *p) {
    u16 r = 0;

    if (p->unk456 > 0)
        r |= 0x20;
    else if (p->unk456 < 0)
        r |= 0x10;
    if ((p->unk456 > 0 && (p->unk5C & 1)) || (p->unk456 < 0 && !(p->unk5C & 1))) {
        p->unk454 = 0;
        p->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea33c;
    }
    return r;
}
