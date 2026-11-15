#include <nitro/types.h>

typedef struct UnkStruct_022f88c0 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f88c0;

extern UnkStruct_022f88c0 *data_ov000_02369b84;

void func_ov000_022f2550(void *p);

void func_ov000_022f88c0(void) {
    func_ov000_022f2550(data_ov000_02369b84->unk218);
}
