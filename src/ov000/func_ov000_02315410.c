#include <nitro/types.h>

typedef struct UnkStruct_02315410 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02315410;

void func_ov000_0231540c(void);

void func_ov000_02315410(UnkStruct_02315410 *s) {
    s->func = func_ov000_0231540c;
}
