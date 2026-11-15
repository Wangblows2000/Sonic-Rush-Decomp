#include <nitro/types.h>

typedef struct UnkStruct_022daa78 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022daa78;

extern UnkStruct_022daa78 *data_ov000_02369b14;

void func_ov000_022d5bb0(void *p);

void func_ov000_022daa78(void) {
    func_ov000_022d5bb0(data_ov000_02369b14->unk218);
}
