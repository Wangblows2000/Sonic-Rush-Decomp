#include <nitro/types.h>

typedef struct UnkStruct_022da8e4_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022da8e4_2;

typedef struct UnkStruct_022da93c {
    u8 pad[0x214];
    UnkStruct_022da8e4_2 *unk214;
} UnkStruct_022da93c;

extern UnkStruct_022da93c *data_ov000_02369b14;

void func_ov000_022d7224(void);

BOOL func_ov000_022da93c(void) {
    return data_ov000_02369b14->unk214->func == func_ov000_022d7224;
}
