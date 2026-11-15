#include <nitro/types.h>

typedef struct UnkStruct_022c6958 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c6958;

void func_ov000_022c6954(void);

void func_ov000_022c6958(UnkStruct_022c6958 *s) {
    s->func = func_ov000_022c6954;
}
