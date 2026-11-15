#include <nitro/types.h>

typedef struct UnkStruct_023093e8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023093e8;

void func_ov000_023093e4(void);

void func_ov000_023093e8(UnkStruct_023093e8 *s) {
    s->func = func_ov000_023093e4;
}
