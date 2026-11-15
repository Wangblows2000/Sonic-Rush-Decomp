#include <nitro/types.h>

typedef struct UnkStruct_02328928 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_02328928;

extern UnkStruct_02328928 *data_022b4574;

void func_0203344c(void *p, s32 a, s32 b);
void func_02032d2c(void *p);

void func_ov000_02328928(void) {
    u8 *p = data_022b4574->unk10;
    func_0203344c(p + 0x94, 0, 0);
    func_02032d2c(p + 0x94);
}
