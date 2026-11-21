#include <nitro/types.h>

typedef struct UnkStruct_022d6cac_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0x10c - 0x54];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6cac_a;

typedef struct UnkStruct_022d6cac UnkStruct_022d6cac;
struct UnkStruct_022d6cac {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6cac *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6cac_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d6cac_r {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022d6cac_r;

typedef struct UnkStruct_022d6cac_b {
    u8 pad[0x218];
    UnkStruct_022d6cac_r *unk218;
} UnkStruct_022d6cac_b;

extern UnkStruct_022d6cac_b *gBoss3;

void Boss3_UpdateCameraShake(u16 *p, s32 a, s32 b);
BOOL Boss3PartCC_CanCatchBall(UnkStruct_022d6cac *s);
s32 Boss3_ArenaDistance(s32 a, s32 b, s32 c);
void Boss3PartCC_State_CatchBall(UnkStruct_022d6cac *s);
void Boss3PartCC_State_JumpOverBall(UnkStruct_022d6cac *s);

void Boss3PartCC_State_WatchBall(UnkStruct_022d6cac *s) {
    Boss3_UpdateCameraShake(&s->unk264, 1, 5);
    if (Boss3PartCC_CanCatchBall(s)) {
        s->unk264 = 0;
        s->func = Boss3PartCC_State_CatchBall;
        return;
    }
    if (Boss3_ArenaDistance(s->unk10, gBoss3->unk218->unk10, (s->unk5C & 1) ? 1 : 0) < 0xfa00)
        s->func = Boss3PartCC_State_JumpOverBall;
}
