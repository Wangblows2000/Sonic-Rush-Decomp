#include <nitro/types.h>

typedef struct UnkStruct_022f18b0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3d6];
    u16 unk442;
} UnkStruct_022f18b0;

void func_ov000_022f1888(void);

void func_ov000_022f18b0(UnkStruct_022f18b0 *s) {
    s->unk442 = 0;
    s->func = func_ov000_022f1888;
}
