#include <nitro/types.h>

typedef struct UnkStruct_0231c3ac_3 {
    u8 unk0;
    u8 pad[0x32c - 1];
    u32 unk32C;
    u32 unk330;
} UnkStruct_0231c3ac_3;

typedef struct UnkStruct_0231c3ac_2 {
    u8 pad[0x204];
    UnkStruct_0231c3ac_3 *unk204;
} UnkStruct_0231c3ac_2;

typedef struct UnkStruct_0231c3ac {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x18 - 0xc];
    void *unk18;
} UnkStruct_0231c3ac;

void func_ov000_0231c808(UnkStruct_0231c3ac_2 *s);
void Player_Action_AttachToObject(UnkStruct_0231c3ac_3 *t, UnkStruct_0231c3ac_2 *s, s32 a, s32 b);

void Mine_OnHit(UnkStruct_0231c3ac *a, UnkStruct_0231c3ac *b) {
    UnkStruct_0231c3ac_2 *s = a->unk18;
    UnkStruct_0231c3ac_3 *t = b->unk18;
    if (s == NULL)
        return;
    if (t == NULL)
        return;
    if (t == NULL || t->unk0 != 1)
        return;
    func_ov000_0231c808(s);
    s->unk204 = t;
    Player_Action_AttachToObject(t, s, 0, 0x50);
    t->unk32C |= 0x2000;
    t->unk330 |= 0x30;
    a->unk18 = NULL;
    a->unkA |= 0x1000;
}
