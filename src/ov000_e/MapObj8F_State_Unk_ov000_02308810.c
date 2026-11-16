#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
    u8 pad210[0x216 - 0x210];
    s16 unk216;
    s16 unk218;
    s16 unk21A;
    s16 unk21C;
} UnkStruct_02308810_T;

void *func_ov000_02307e84(void);
UnkStruct_02308810_T *func_ov000_0234237c(s32 a, void *b);
BOOL func_ov000_02307dfc(void *a, s32 b);
void func_ov000_02308800(UnkStruct_StageObj *s);

void MapObj8F_State_Unk_ov000_02308810(UnkStruct_StageObj *s) {
    UnkStruct_02308810_T *t = func_ov000_0234237c(1, func_ov000_02307e84());

    if (func_ov000_02307dfc(t, 0)) {
        s->unk1F4 &= ~0x10;
        s->unk54 |= 4;
        return;
    }
    if (t == NULL)
        return;
    if (!(s->unk1E0->unk4 & 2)) {
        t->unk216 = s->unk10 >> 8;
        t->unk218 = s->unk14 >> 8;
    } else {
        t->unk21A = s->unk10 >> 8;
        t->unk21C = s->unk14 >> 8;
    }
    if (t->unk20C & 8)
        func_ov000_02308800(s);
}
