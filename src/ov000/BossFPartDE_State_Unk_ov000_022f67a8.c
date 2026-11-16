#include <nitro/types.h>

typedef struct UnkStruct_022f67a8 {
    u8 pad[0x14];
    s32 unk14;
    u8 pad1[0x50];
    void (*func)(void);
} UnkStruct_022f67a8;

void func_ov000_022f66e8(void);

void BossFPartDE_State_Unk_ov000_022f67a8(UnkStruct_022f67a8 *s) {
    if (s->unk14 < -0x1f400)
        s->func = func_ov000_022f66e8;
}
