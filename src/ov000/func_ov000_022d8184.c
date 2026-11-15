#include <nitro/types.h>

typedef struct UnkStruct_022d8184 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d8184;

void func_ov000_022d7fd4(void);

void func_ov000_022d8184(UnkStruct_022d8184 *s) {
    s->func = func_ov000_022d7fd4;
}
