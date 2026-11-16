#include <nitro/types.h>

typedef struct UnkStruct_0231a364_3 {
    u8 pad[0x2a];
    s16 unk2A;
} UnkStruct_0231a364_3;

typedef struct UnkStruct_0231a364_2 {
    u8 pad[0x13e];
    u16 unk13E;
} UnkStruct_0231a364_2;

typedef struct UnkStruct_0231a364 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_0231a364;

void Trampoline_Action_Bounce(UnkStruct_0231a364_2 *s);
void Player_Action_SpringLaunch(UnkStruct_0231a364_3 *t, s32 a, s32 b);
void Player_SetSourceMapObject(void *a, UnkStruct_0231a364_2 *s);

void Trampoline_OnHit(UnkStruct_0231a364 *a, UnkStruct_0231a364 *b) {
    UnkStruct_0231a364_2 *s = a->unk18;
    UnkStruct_0231a364_3 *t;
    s16 v;
    Trampoline_Action_Bounce(s);
    s->unk13E &= ~0x100;
    t = b->unk18;
    v = -(t->unk2A + (t->unk2A >> 3) + (t->unk2A >> 4));
    if (t == NULL)
        return;
    if (v < -0xc00)
        v = -0xc00;
    else if (v > -0x780)
        v = -0x780;
    Player_Action_SpringLaunch(t, 0, v);
    Player_SetSourceMapObject(b->unk18, s);
}
