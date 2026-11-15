#include <nitro/types.h>

typedef struct UnkStruct_0230ba28 {
    u8 pad[0xc8];
    u32 unkC8;
} UnkStruct_0230ba28;

extern UnkStruct_0230ba28 data_ov000_0236aad8;

void func_ov000_022fb9c8(void *p);

void func_ov000_0230ba28(void *p) {
    data_ov000_0236aad8.unkC8 &= ~0x40000;
    func_ov000_022fb9c8(p);
}
