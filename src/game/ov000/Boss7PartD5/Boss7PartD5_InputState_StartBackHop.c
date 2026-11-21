#include <nitro/types.h>

typedef struct UnkStruct_022ea3dc {
    u8 pad0[0x5c];
    u16 unk5C;
    u8 pad5E[0x43c - 0x5e];
    void (*unk43C)(struct UnkStruct_022ea3dc *p);
    u8 pad440[0x456 - 0x440];
    s16 unk456;
} UnkStruct_022ea3dc;

void Boss7PartD5_InputState_TurnAround(UnkStruct_022ea3dc *p);
void Boss7PartD5_InputState_TapBack(UnkStruct_022ea3dc *p);

void Boss7PartD5_InputState_StartBackHop(UnkStruct_022ea3dc *p) {
    if ((p->unk456 > 0 && p != NULL && !(p->unk5C & 1)) || (p->unk456 < 0 && p != NULL && (p->unk5C & 1)))
        p->unk43C = Boss7PartD5_InputState_TurnAround;
    else
        p->unk43C = Boss7PartD5_InputState_TapBack;
    p->unk43C(p);
}
