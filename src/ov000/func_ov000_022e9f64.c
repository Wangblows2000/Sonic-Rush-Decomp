#include <nitro/types.h>

typedef struct UnkStruct_022e9f64 {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x16];
    s16 unk456;
} UnkStruct_022e9f64;

void func_ov000_022ea798(void);

s32 func_ov000_022e9f64(UnkStruct_022e9f64 *s) {
    if (s->unk456-- == 0)
        s->func = func_ov000_022ea798;
    return 0x100;
}
