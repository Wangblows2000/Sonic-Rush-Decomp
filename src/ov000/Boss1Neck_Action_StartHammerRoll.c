#include <nitro/types.h>

typedef struct UnkStruct_022c8d54 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c8d54;

void Boss1Neck_State_RollStart(void);

void Boss1Neck_Action_StartHammerRoll(UnkStruct_022c8d54 *s) {
    s->func = Boss1Neck_State_RollStart;
}
