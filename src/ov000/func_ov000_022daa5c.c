#include <nitro/types.h>

typedef struct UnkStruct_022daa5c {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022daa5c;

extern UnkStruct_022daa5c *data_ov000_02369b14;

void func_ov000_022d5acc(void *p);

void func_ov000_022daa5c(void) {
    func_ov000_022d5acc(data_ov000_02369b14->unk218);
}
