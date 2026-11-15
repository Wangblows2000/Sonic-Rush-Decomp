#include <nitro/types.h>

typedef struct UnkStruct_02301728 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02301728;

void func_ov000_023016c4(void);

void func_ov000_02301728(UnkStruct_02301728 *s) {
    s->func = func_ov000_023016c4;
}
