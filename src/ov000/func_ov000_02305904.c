#include <nitro/types.h>

typedef struct UnkStruct_02305904 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02305904;

void func_ov000_023056fc(void);

void func_ov000_02305904(UnkStruct_02305904 *s) {
    s->func = func_ov000_023056fc;
}
