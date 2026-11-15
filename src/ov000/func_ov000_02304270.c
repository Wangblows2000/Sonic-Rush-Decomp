#include <nitro/types.h>

typedef struct UnkStruct_02304270 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02304270;

void func_ov000_0230422c(void);

void func_ov000_02304270(UnkStruct_02304270 *s) {
    s->func = func_ov000_0230422c;
}
