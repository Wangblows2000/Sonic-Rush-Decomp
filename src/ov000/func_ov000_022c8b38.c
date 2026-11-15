#include <nitro/types.h>

typedef struct UnkStruct_022c8b38 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x334 - 0x6c];
    u8 unk334;
} UnkStruct_022c8b38;

void func_ov000_022c8af8(void);

void func_ov000_022c8b38(UnkStruct_022c8b38 *s) {
    if (s->unk334 & 2)
        s->func = func_ov000_022c8af8;
}
