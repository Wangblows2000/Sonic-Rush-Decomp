#include <nitro/types.h>

typedef struct UnkStruct_0230d260 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230d260;

void func_ov000_0230d25c(void);

void func_ov000_0230d260(UnkStruct_0230d260 *s) {
    s->func = func_ov000_0230d25c;
}
