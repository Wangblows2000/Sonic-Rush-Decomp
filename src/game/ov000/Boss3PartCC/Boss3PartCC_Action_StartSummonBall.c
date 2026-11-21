#include <nitro/types.h>

typedef struct UnkStruct_022d8ba8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d8ba8;

void Boss3PartCC_State_SummonBall(void);

void Boss3PartCC_Action_StartSummonBall(UnkStruct_022d8ba8 *s) {
    s->func = Boss3PartCC_State_SummonBall;
}
