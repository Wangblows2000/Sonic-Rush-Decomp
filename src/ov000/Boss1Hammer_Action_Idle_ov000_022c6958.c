#include <nitro/types.h>

typedef struct UnkStruct_022c6958 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c6958;

void Boss1Hammer_State_Idle_ov000_022c6954(void);

void Boss1Hammer_Action_Idle_ov000_022c6958(UnkStruct_022c6958 *s) {
    s->func = Boss1Hammer_State_Idle_ov000_022c6954;
}
