#include <nitro/types.h>

typedef struct UnkStruct_022f8888 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8888;

extern UnkStruct_022f8888 *data_ov000_02369b84;

void func_ov000_022f1aac(void *p);

void func_ov000_022f8888(void) {
    func_ov000_022f1aac(data_ov000_02369b84->unk218);
}
