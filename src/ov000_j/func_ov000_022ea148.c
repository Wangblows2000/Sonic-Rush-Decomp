#include <nitro/types.h>

typedef struct {
    u8 pad0[0x43c];
    void *unk43C;
    u8 pad440[0x456 - 0x440];
    s16 unk456;
} UnkStruct_022ea148;

void Boss7PartD5_InputState_Unk_ov000_022ea798(UnkStruct_022ea148 *p);

u16 func_ov000_022ea148(UnkStruct_022ea148 *p) {
    u16 r = 0;

    if (p->unk456 > 0) {
        p->unk456--;
    } else if (p->unk456 < 0) {
        p->unk456++;
    } else {
        r |= 0x40;
        p->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea798;
    }
    return r;
}
