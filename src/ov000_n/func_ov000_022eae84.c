#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022eae84 UnkStruct_022eae84;
struct UnkStruct_022eae84 {
    UnkStruct_StageObj base;
    u8 pad20C[0x43C - 0x20C];
    void (*unk43C)(UnkStruct_022eae84 *s);
    u8 pad440[0x456 - 0x440];
    s16 unk456;
    u16 unk458;
};

extern UnkStruct_StageObj *gPlayers;

void Boss7PartD5_InputState_Unk_ov000_022ea25c(UnkStruct_022eae84 *s);
void Boss7PartD5_InputState_Unk_ov000_022ea614(UnkStruct_022eae84 *s);
void Boss7PartD5_InputState_Unk_ov000_022ea638(UnkStruct_022eae84 *s);
void Boss7PartD5_InputState_Unk_ov000_022ea3dc(UnkStruct_022eae84 *s);

void func_ov000_022eae84(UnkStruct_022eae84 *s) {
    UnkStruct_StageObj *p = gPlayers;
    s32 x = s->base.unk10;
    s32 dx = p->unk10 - x;
    s32 adx = dx < 0 ? -dx : dx;
    s32 dy = s->base.unk14 - p->unk14;

    if (adx < 0x3000) {
        if (x > 0xa000)
            s->unk456 = -((Rand() & 0xf) + 0xf);
        else
            s->unk456 = (Rand() & 0xf) + 0xf;
        s->unk458 = (Rand() & 0x3f) + 0x3c;
        if (!(Rand() & 1))
            s->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea614;
        else
            s->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea25c;
        return;
    }
    if (adx < 0x4000 && dy > 0x3000 && p->unk30 < 0) {
        s->unk456 = dx >> 8;
        s->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea638;
        return;
    }
    if ((s->base.unk5C & 1 ? 0x14000 - x : x) > 0x4000 && !(Rand() & 1)) {
        if (s->base.unk10 < p->unk10)
            s->unk456 = -1;
        else
            s->unk456 = 1;
        s->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea3dc;
        return;
    }
    if (s->base.unk10 > 0xa000)
        s->unk456 = -((Rand() & 0xf) + 0xf);
    else
        s->unk456 = (Rand() & 0xf) + 0xf;
    s->unk458 = (Rand() & 0x3f) + 0x3c;
    s->unk43C = Boss7PartD5_InputState_Unk_ov000_022ea25c;
}
