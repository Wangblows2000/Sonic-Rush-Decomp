#include <nitro/types.h>

typedef struct UnkStruct_022e4474_2 {
    u8 pad[0xd5e];
    u16 unkD5E;
    u8 pad1[0x111c - 0xd60];
    void (*unk111C)(void);
} UnkStruct_022e4474_2;

typedef struct UnkStruct_022e4474 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x240 - 0x6c];
    u8 unk240;
    u8 pad2[0x42c - 0x241];
    u16 unk42C;
    u16 unk42E;
    u16 unk430;
    u16 unk432;
    u16 unk434;
    u16 unk436;
} UnkStruct_022e4474;

extern UnkStruct_022e4474_2 *gBoss6;

void Player_SetAnimation(UnkStruct_022e4474 *s, s32 a);

void Boss6_State_PlayerStunned(UnkStruct_022e4474 *s) {
    if (!(s->unk240 & 2))
        return;
    s->func = gBoss6->unk111C;
    Player_SetAnimation(s, 0x3c);
    s->unk434 = 1;
    s->unk436 = 2;
    s->unk430 = 0x40;
    s->unk432 = 0x80;
    if (gBoss6->unkD5E != 0) {
        s->unk42C = 0x20;
        s->unk42E = 0x10;
    } else {
        s->unk42C = 0x10;
        s->unk42E = 0x20;
    }
}
