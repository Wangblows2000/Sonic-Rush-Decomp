#include <nitro/types.h>

typedef struct UnkStruct_0231797c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231797c;

void func_ov000_023177e8(void);

void Swing_Action_Init(UnkStruct_0231797c *s) {
    s->func = func_ov000_023177e8;
}
