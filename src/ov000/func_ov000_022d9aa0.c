#include <nitro/types.h>

typedef struct UnkStruct_022d9aa0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9aa0;

void func_ov000_022d98ec(void);

void func_ov000_022d9aa0(UnkStruct_022d9aa0 *s) {
    s->func = func_ov000_022d98ec;
}
