#include <nitro/types.h>

typedef struct UnkStruct_0230ca40 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230ca40;

void func_ov000_0230c89c(void);

void func_ov000_0230ca40(UnkStruct_0230ca40 *s) {
    s->func = func_ov000_0230c89c;
}
