#include <nitro/types.h>

typedef struct UnkStruct_022f5840 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x58];
    void (*func)(void);
} UnkStruct_022f5840;

void func_ov000_022f57f8(void);

void BossFPartDE_State_Unk_ov000_022f5840(UnkStruct_022f5840 *s) {
    if (s->unkC++ > 0)
        s->func = func_ov000_022f57f8;
}
