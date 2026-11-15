#include <nitro/types.h>

typedef struct UnkStruct_022c74d4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c74d4;

void func_ov000_022c74d0(void);

void func_ov000_022c74d4(UnkStruct_022c74d4 *s) {
    s->func = func_ov000_022c74d0;
}
