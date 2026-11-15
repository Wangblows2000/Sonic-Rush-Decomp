#include <nitro/types.h>

typedef struct UnkStruct_023216a4 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x9c - 0x6c];
    u16 unk9C;
} UnkStruct_023216a4;

void func_ov000_02321514(void);
void func_02069818(UnkStruct_023216a4 *s, u16 a);

void func_ov000_023216a4(UnkStruct_023216a4 *s) {
    s->func = func_ov000_02321514;
    func_02069818(s, s->unk9C + 1);
}
