#include <nitro/types.h>

typedef struct UnkStruct_022f88a4 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f88a4;

extern UnkStruct_022f88a4 *data_ov000_02369b84;

void func_ov000_022f2444(void *p);

void func_ov000_022f88a4(void) {
    func_ov000_022f2444(data_ov000_02369b84->unk218);
}
