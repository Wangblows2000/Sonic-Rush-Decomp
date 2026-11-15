#include <nitro/types.h>

typedef struct UnkStruct_022f6654 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_022f6654;

void func_ov000_022f6b8c(UnkStruct_022f6654 *s);
void func_ov000_022f65fc(void);

void func_ov000_022f6654(UnkStruct_022f6654 *s) {
    func_ov000_022f6b8c(s);
    if (s->unkC++ > 0x1e)
        s->func = func_ov000_022f65fc;
}
