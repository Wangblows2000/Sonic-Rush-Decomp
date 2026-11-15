#include <nitro/types.h>

typedef struct UnkStruct_0230b320 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230b320;

void func_02069818(UnkStruct_0230b320 *s, s32 a);
void func_ov000_0230b1c8(void);

void func_ov000_0230b320(UnkStruct_0230b320 *s) {
    func_02069818(s, 0);
    s->func = func_ov000_0230b1c8;
}
