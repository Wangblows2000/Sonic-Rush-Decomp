#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc114 {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc114;

extern UnkStruct_022cc114 *data_ov000_02369acc;

void func_ov000_022c74f8(void);

BOOL func_ov000_022cc114(void) {
    return func_ov000_022c74f8 == data_ov000_02369acc->unk210->func;
}
