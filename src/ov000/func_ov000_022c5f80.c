#include <nitro/types.h>

typedef struct UnkStruct_022c5f80 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c5f80;

void func_ov000_022c5f7c(void);

void func_ov000_022c5f80(UnkStruct_022c5f80 *s) {
    s->func = func_ov000_022c5f7c;
}
