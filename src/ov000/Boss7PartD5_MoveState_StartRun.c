#include <nitro/types.h>

typedef struct UnkStruct_022e9798 {
    u8 pad[0x440];
    void (*func)(void);
    u8 pad1[0x464 - 0x444];
    u16 unk464;
    u16 unk466;
} UnkStruct_022e9798;

extern s32 gBossStatus[];

void Boss7PartD5_MoveState_Run(void);

void Boss7PartD5_MoveState_StartRun(UnkStruct_022e9798 *s) {
    s->unk464 = 6;
    s->unk466 = 1;
    gBossStatus[1] = 0x25;
    s->func = Boss7PartD5_MoveState_Run;
}
