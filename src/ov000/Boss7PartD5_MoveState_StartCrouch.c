#include <nitro/types.h>

typedef struct UnkStruct_022e91d0 UnkStruct_022e91d0;
struct UnkStruct_022e91d0 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x58 - 0x30];
    u32 unk58;
    u8 pad2[0x440 - 0x5c];
    void (*func)(UnkStruct_022e91d0 *s);
};

extern s32 gBossStatus[];

void Boss7_SetRivalAnimation(UnkStruct_022e91d0 *s, s32 a);
void Boss7_SetCollidersEnabled(UnkStruct_022e91d0 *s, s32 a, s32 b);
void Boss7PartD5_MoveState_Crouch(UnkStruct_022e91d0 *s);

void Boss7PartD5_MoveState_StartCrouch(UnkStruct_022e91d0 *s) {
    s->unk58 |= 0x10;
    s->unk2E = 0;
    Boss7_SetRivalAnimation(s, 3);
    Boss7_SetCollidersEnabled(s, 1, 0);
    gBossStatus[1] = 0x27;
    s->func = Boss7PartD5_MoveState_Crouch;
    s->func(s);
}
