#include <nitro/types.h>

typedef struct UnkStruct_022c95a8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c95a8;

void Boss1Neck_State_Unk_ov000_022c94a0(void);

void Boss1Neck_Action_Unk_ov000_022c95a8(UnkStruct_022c95a8 *s) {
    s->func = Boss1Neck_State_Unk_ov000_022c94a0;
}
