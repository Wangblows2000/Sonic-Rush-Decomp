#include <nitro/types.h>

typedef struct UnkStruct_022c74d4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c74d4;

void Boss1Neck_State_Defeated(void);

void Boss1Neck_Action_Defeat(UnkStruct_022c74d4 *s) {
    s->func = Boss1Neck_State_Defeated;
}
