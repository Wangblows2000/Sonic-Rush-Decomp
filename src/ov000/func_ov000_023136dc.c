#include <nitro/types.h>

typedef struct UnkStruct_023136dc {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023136dc;

void func_ov000_023136bc(void);

void func_ov000_023136dc(UnkStruct_023136dc *s) {
    s->func = func_ov000_023136bc;
}
