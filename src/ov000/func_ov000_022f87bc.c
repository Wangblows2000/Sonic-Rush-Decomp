#include <nitro/types.h>

typedef struct UnkStruct_022f87bc {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f87bc;

extern UnkStruct_022f87bc *data_ov000_02369b84;

void func_ov000_022ef7b0(void *p);

void func_ov000_022f87bc(void) {
    func_ov000_022ef7b0(data_ov000_02369b84->unk218);
}
