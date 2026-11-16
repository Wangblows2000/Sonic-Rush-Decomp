#include <nitro/types.h>

typedef struct UnkStruct_0230d260 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230d260;

void IronBall_State_Unk_ov000_0230d25c(void);

void IronBall_Action_Unk_ov000_0230d260(UnkStruct_0230d260 *s) {
    s->func = IronBall_State_Unk_ov000_0230d25c;
}
