#include <nitro/types.h>

typedef struct UnkStruct_022cb088 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cb088;

extern s32 gBossStatus[];

void Boss1_State_WaitBigSlam(void);

void Boss1_Action_WaitBigSlam(UnkStruct_022cb088 *s) {
    gBossStatus[1] = 3;
    s->func = Boss1_State_WaitBigSlam;
}
