#include <nitro/types.h>

typedef struct UnkStruct_022dac60 {
    u8 pad[0x214];
    void *unk214;
} UnkStruct_022dac60;

extern UnkStruct_022dac60 *data_ov000_02369b14;

void func_ov000_022d9e1c(UnkStruct_022dac60 *s);
void func_ov000_022d8bf8(void *p);

void func_ov000_022dac60(void) {
    func_ov000_022d9e1c(data_ov000_02369b14);
    func_ov000_022d8bf8(data_ov000_02369b14->unk214);
}
