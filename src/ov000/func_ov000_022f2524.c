#include <nitro/types.h>

typedef struct UnkStruct_022f2524 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f2524;

void func_ov000_022f4ce4(UnkStruct_022f2524 *s, s32 a, s32 b, s32 c);
void func_ov000_022f24f8(void);

void func_ov000_022f2524(UnkStruct_022f2524 *s) {
    func_ov000_022f4ce4(s, 0x19, 0x1000, 0);
    s->func = func_ov000_022f24f8;
}
