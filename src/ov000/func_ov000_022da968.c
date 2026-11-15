#include <nitro/types.h>

typedef struct UnkStruct_022da8e4_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022da8e4_2;

typedef struct UnkStruct_022da968 {
    u8 pad[0x214];
    UnkStruct_022da8e4_2 *unk214;
} UnkStruct_022da968;

extern UnkStruct_022da968 *data_ov000_02369b14;

void func_ov000_022d8194(void);

BOOL func_ov000_022da968(void) {
    return data_ov000_02369b14->unk214->func == func_ov000_022d8194;
}
