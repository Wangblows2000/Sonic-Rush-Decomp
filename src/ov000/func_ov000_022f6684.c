#include <nitro/types.h>

typedef struct UnkStruct_022f6684 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad2[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_022f6684;

void BossFPartDE_State_Unk_ov000_022f6654(void);

void func_ov000_022f6684(UnkStruct_022f6684 *s) {
    s->unkC = 0;
    s->func = BossFPartDE_State_Unk_ov000_022f6654;
}
