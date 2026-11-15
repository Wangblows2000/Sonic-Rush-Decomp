#include <nitro/types.h>

typedef struct UnkStruct_022f8948 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f8948;

extern UnkStruct_022f8948 *data_ov000_02369b84;

void func_ov000_022f3588(void *p);

void func_ov000_022f8948(void) {
    func_ov000_022f3588(data_ov000_02369b84->unk218);
}
