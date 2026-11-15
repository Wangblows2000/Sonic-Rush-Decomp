#include <nitro/types.h>

typedef struct UnkStruct_022f898c {
    u8 pad[0x218];
    void *unk218;
} UnkStruct_022f898c;

extern UnkStruct_022f898c *data_ov000_02369b84;

void func_ov000_022f3ed4(void *p);

void func_ov000_022f898c(void) {
    func_ov000_022f3ed4(data_ov000_02369b84->unk218);
}
