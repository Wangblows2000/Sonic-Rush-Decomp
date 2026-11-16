#include <nitro/types.h>

typedef struct {
    u8 pad[0x10];
    s32 mode;
    u8 pad14[8];
    u8 kind;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
void func_020480ec(void);
void func_02048864(void);
void func_0204b528(void);

void func_0204eb04(void) {
    if (data_022c4560.mode == 2) {
        func_020480ec();
        return;
    }
    switch (data_022c4560.kind) {
    case 7:
        func_020480ec();
        return;
    case 8:
        func_02048864();
        return;
    default:
        func_0204b528();
        func_02048864();
        return;
    }
}
