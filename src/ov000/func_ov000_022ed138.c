#include <nitro/types.h>

typedef struct UnkStruct_022ed138 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1c4];
    u16 unk230;
} UnkStruct_022ed138;

void func_ov000_022ed0d8(void);

void func_ov000_022ed138(UnkStruct_022ed138 *s) {
    if (s->unk230++ > 10)
        s->func = func_ov000_022ed0d8;
}
