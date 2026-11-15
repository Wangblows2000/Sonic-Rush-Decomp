#include <nitro/types.h>

typedef struct UnkStruct_022c8d54 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c8d54;

void func_ov000_022c8d20(void);

void func_ov000_022c8d54(UnkStruct_022c8d54 *s) {
    s->func = func_ov000_022c8d20;
}
