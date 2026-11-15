#include <nitro/types.h>

typedef struct UnkStruct_022ee17c {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022ee17c;

extern UnkStruct_022ee17c *data_ov000_02369b78;

void func_ov000_022eb394(void *p);

void func_ov000_022ee17c(void) {
    func_ov000_022eb394(data_ov000_02369b78->unk214);
}
