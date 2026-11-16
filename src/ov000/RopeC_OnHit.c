#include <nitro/types.h>

typedef struct UnkStruct_02314dc8_3 {
    u8 pad[0xa0];
    s32 unkA0;
} UnkStruct_02314dc8_3;

typedef struct UnkStruct_02314dc8_2 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x128 - 0x5e];
    UnkStruct_02314dc8_3 *unk128;
    u8 pad3[0x204 - 0x12c];
    u8 *unk204;
} UnkStruct_02314dc8_2;

typedef struct UnkStruct_02314dc8 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_02314dc8;

void func_020319a4(void *p, s32 a, s32 b, s32 c, s32 d);
void Player_Action_RopeC(u8 *t, UnkStruct_02314dc8_2 *s, s32 a);

void RopeC_OnHit(UnkStruct_02314dc8 *a, UnkStruct_02314dc8 *b) {
    UnkStruct_02314dc8_2 *s = a->unk18;
    u8 *t;
    if (s->unk204 != NULL)
        return;
    t = b->unk18;
    if (t == NULL)
        return;
    s->unk204 = t;
    s->unk54 |= 2;
    func_020319a4(s->unk128, s->unk128->unkA0, 0, 0, 0);
    if (*t != 1)
        return;
    s->unk5C &= ~0x200;
    Player_Action_RopeC(t, s, 0xa000);
}
