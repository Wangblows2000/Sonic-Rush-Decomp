#include <nitro/types.h>

typedef struct UnkStruct_02317db8_3 {
    u8 pad[8];
    u8 unk8;
} UnkStruct_02317db8_3;

typedef struct UnkStruct_02317db8_2 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x1e0 - 0x58];
    UnkStruct_02317db8_3 *unk1E0;
    u8 pad2[0x1f4 - 0x1e4];
    u32 unk1F4;
    u8 pad3[0x204 - 0x1f8];
    u8 *unk204;
} UnkStruct_02317db8_2;

typedef struct UnkStruct_02317db8 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_02317db8;

void Player_Action_SwingSky(u8 *t, UnkStruct_02317db8_2 *s, s32 a);

void SwingSky_OnHit(UnkStruct_02317db8 *a, UnkStruct_02317db8 *b) {
    UnkStruct_02317db8_2 *s = a->unk18;
    u8 *t = b->unk18;
    if (s == NULL)
        return;
    if (t == NULL && *t != 1)
        return;
    s->unk1F4 |= 1;
    s->unk54 |= 2;
    s->unk204 = t;
    if (s->unk1E0->unk8 > 0x80)
        Player_Action_SwingSky(t, s, 0);
    else
        Player_Action_SwingSky(t, s, 1);
}
