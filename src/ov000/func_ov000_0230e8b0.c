#include <nitro/types.h>

typedef struct UnkStruct_0230e8b0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230e8b0;

void JetCatapult_State_Unk_ov000_0230e89c(void);

void func_ov000_0230e8b0(UnkStruct_0230e8b0 *s) {
    s->func = JetCatapult_State_Unk_ov000_0230e89c;
}
