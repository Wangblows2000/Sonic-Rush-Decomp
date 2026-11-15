#include <nitro/types.h>

typedef struct UnkStruct_022c8d20 UnkStruct_022c8d20;
struct UnkStruct_022c8d20 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c8d20 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
};

void func_ov000_022c8ca4(UnkStruct_022c8d20 *s);

void func_ov000_022c8d20(UnkStruct_022c8d20 *s) {
    s->unk20C = 0;
    s->func = func_ov000_022c8ca4;
    s->func(s);
}
