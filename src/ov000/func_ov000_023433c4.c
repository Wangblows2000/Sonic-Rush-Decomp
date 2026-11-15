#include <nitro/types.h>

typedef struct UnkStruct_023433c4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023433c4;

void func_ov000_02343310(void);

void func_ov000_023433c4(UnkStruct_023433c4 *s) {
    s->func = func_ov000_02343310;
}
