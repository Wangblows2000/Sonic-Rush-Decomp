#include <nitro/types.h>

typedef struct UnkStruct_0230ddf0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230ddf0;

void func_ov000_0230ddac(void);

void func_ov000_0230ddf0(UnkStruct_0230ddf0 *s) {
    s->func = func_ov000_0230ddac;
}
