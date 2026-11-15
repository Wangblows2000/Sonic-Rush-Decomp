#include <nitro/types.h>

typedef struct UnkStruct_023099b0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023099b0;

void func_ov000_02309850(void);

void func_ov000_023099b0(UnkStruct_023099b0 *s) {
    s->func = func_ov000_02309850;
}
