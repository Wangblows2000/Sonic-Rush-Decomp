#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x40];
    u16 unk40;
    u8 pad42[0x32c - 0x42];
    u32 unk32C;
} UnkPlayer_0231bbb0;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_0231bbb0;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

void func_ov000_02328e6c(UnkPlayer_0231bbb0 *p, s32 a, s32 b);

void func_ov000_0231bbb0(UnkColl_0231bbb0 *a, UnkColl_0231bbb0 *b) {
    UnkStruct_StageObj *o = a->unk18;
    UnkPlayer_0231bbb0 *p = b->unk18;

    if (p == NULL)
        return;
    if (o->unk1E0->unk4 & 1)
        p->unk40 = 0xa8;
    else
        p->unk40 = 0x54;
    p->unk32C |= 0x2000000;
    if (!(gGameState.unk20 & 7))
        func_ov000_02328e6c(p, 0, -12);
}
