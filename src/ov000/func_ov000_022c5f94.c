#include <nitro/types.h>

typedef struct UnkStruct_022c5f94 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c5f94;

void func_ov000_022c5f90(void);

void func_ov000_022c5f94(UnkStruct_022c5f94 *s) {
    s->func = func_ov000_022c5f90;
}
