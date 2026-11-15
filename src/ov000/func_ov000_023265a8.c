#include <nitro/types.h>

typedef struct UnkStruct_023265a8_3 {
    u8 pad[0x54];
    u32 unk54;
} UnkStruct_023265a8_3;

typedef struct UnkStruct_023265a8_2 {
    u8 pad[0x164];
    UnkStruct_023265a8_3 *unk164;
} UnkStruct_023265a8_2;

typedef struct UnkStruct_023265a8 {
    u8 pad[0x10];
    UnkStruct_023265a8_2 *unk10;
} UnkStruct_023265a8;

extern UnkStruct_023265a8 *data_022b4574;

void func_ov000_023265a8(void) {
    UnkStruct_023265a8_2 *p = data_022b4574->unk10;
    if (p->unk164 == NULL)
        return;
    if (p->unk164->unk54 & 4)
        p->unk164 = NULL;
}
