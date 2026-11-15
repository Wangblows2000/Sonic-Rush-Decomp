#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc16c {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc16c;

extern UnkStruct_022cc16c *data_ov000_02369acc;

void func_ov000_022c895c(void);

BOOL func_ov000_022cc16c(void) {
    return func_ov000_022c895c == data_ov000_02369acc->unk210->func;
}
