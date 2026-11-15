#include <nitro/types.h>

typedef struct UnkStruct_022f2ee0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f2ee0;

void func_ov000_022f4ce4(UnkStruct_022f2ee0 *s, s32 a, s32 b, s32 c);
void func_ov000_022f2eb4(void);

void func_ov000_022f2ee0(UnkStruct_022f2ee0 *s) {
    func_ov000_022f4ce4(s, 0x13, 0x1000, 0);
    s->func = func_ov000_022f2eb4;
}
