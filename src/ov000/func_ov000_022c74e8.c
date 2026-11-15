#include <nitro/types.h>

typedef struct UnkStruct_022c74e8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c74e8;

void func_ov000_022c74e4(void);

void func_ov000_022c74e8(UnkStruct_022c74e8 *s) {
    s->func = func_ov000_022c74e4;
}
