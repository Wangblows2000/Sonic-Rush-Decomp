#include <nitro/types.h>

typedef struct UnkStruct_022dac2c {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022dac2c;

extern UnkStruct_022dac2c *data_ov000_02369b14;

void func_ov000_022d9da0(UnkStruct_022dac2c *s);
void func_ov000_022d8ba8(void *p);

void func_ov000_022dac2c(void) {
    func_ov000_022d9da0(data_ov000_02369b14);
    func_ov000_022d8ba8(data_ov000_02369b14->unk214);
}
