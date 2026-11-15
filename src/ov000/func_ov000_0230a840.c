#include <nitro/types.h>

typedef struct UnkStruct_0230a840 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230a840;

void func_02069818(UnkStruct_0230a840 *s, s32 a);
void func_ov000_0230a7f0(void);

void func_ov000_0230a840(UnkStruct_0230a840 *s) {
    func_02069818(s, 8);
    s->func = func_ov000_0230a7f0;
}
