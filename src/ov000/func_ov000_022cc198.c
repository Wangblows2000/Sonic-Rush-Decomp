#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc198 {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc198;

extern UnkStruct_022cc198 *data_ov000_02369acc;

void func_ov000_022c8d64(void);

BOOL func_ov000_022cc198(void) {
    return func_ov000_022c8d64 == data_ov000_02369acc->unk210->func;
}
