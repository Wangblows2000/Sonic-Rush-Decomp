#include <nitro/types.h>

typedef struct UnkStruct_022f8310 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f8310;

void func_ov000_022f8224(void);

void func_ov000_022f8310(UnkStruct_022f8310 *s) {
    s->func = func_ov000_022f8224;
}
