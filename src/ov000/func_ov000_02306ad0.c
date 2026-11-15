#include <nitro/types.h>

typedef struct UnkStruct_02306ad0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02306ad0;

void func_ov000_023064d8(void);

void func_ov000_02306ad0(UnkStruct_02306ad0 *s) {
    s->func = func_ov000_023064d8;
}
