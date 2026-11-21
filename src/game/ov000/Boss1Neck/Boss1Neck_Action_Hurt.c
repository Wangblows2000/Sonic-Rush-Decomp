#include <nitro/types.h>

typedef struct UnkStruct_022c74e8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c74e8;

void Boss1Neck_State_Hurt(void);

void Boss1Neck_Action_Hurt(UnkStruct_022c74e8 *s) {
    s->func = Boss1Neck_State_Hurt;
}
