#include <nitro/types.h>

typedef struct UnkStruct_022daa94 {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022daa94;

extern UnkStruct_022daa94 *data_ov000_02369b14;

void func_ov000_022d5cc4(void *p);

void func_ov000_022daa94(void) {
    func_ov000_022d5cc4(data_ov000_02369b14->unk218);
}
