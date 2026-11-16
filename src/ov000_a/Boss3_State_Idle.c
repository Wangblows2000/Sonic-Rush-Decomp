#include <nitro/types.h>

typedef struct UnkStruct_022d9dc0 UnkStruct_022d9dc0;
struct UnkStruct_022d9dc0 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022d9dc0 *s);
    u8 pad1[0x210 - 0x6c];
    s32 unk210;
    void *unk214;
    void *unk218;
    u8 pad2[0x224 - 0x21c];
    u16 unk224;
};

typedef struct UnkStruct_022d9dc0_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022d9dc0_7;

extern UnkStruct_022d9dc0_7 gBossStatus;

void Boss3_StartSummonBall(void);
void Boss3_SelectNextAttack(void);

void Boss3_State_Idle(UnkStruct_022d9dc0 *s) {
    if (s->unk224++ <= 0x1e)
        return;
    s->unk210 = 0;
    if (s->unk218 == NULL)
        Boss3_StartSummonBall();
    else
        Boss3_SelectNextAttack();
}
