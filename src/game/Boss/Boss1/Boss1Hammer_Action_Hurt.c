#include <nitro/types.h>

typedef struct UnkStruct_022c5f80 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c5f80;

void Boss1Hammer_State_Hurt(void);

void Boss1Hammer_Action_Hurt(UnkStruct_022c5f80 *s) {
    s->func = Boss1Hammer_State_Hurt;
}
