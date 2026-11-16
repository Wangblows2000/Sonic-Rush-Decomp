#include <nitro/types.h>

typedef struct UnkStruct_022c9da8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c9da8;

void Boss1Neck_State_Unk_ov000_022c9d9c(void);

void Boss1Neck_Action_Unk_ov000_022c9da8(UnkStruct_022c9da8 *s) {
    s->func = Boss1Neck_State_Unk_ov000_022c9d9c;
}
