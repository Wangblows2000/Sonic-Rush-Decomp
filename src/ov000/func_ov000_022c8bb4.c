#include <nitro/types.h>

typedef struct UnkStruct_022c8bb4 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x20c - 0x6c];
    u16 unk20c;
} UnkStruct_022c8bb4;

void func_ov000_022c8b90(void);

void func_ov000_022c8bb4(UnkStruct_022c8bb4 *s) {
    s->unk20c = 0;
    s->func = func_ov000_022c8b90;
}
