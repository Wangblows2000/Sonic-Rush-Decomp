#include <nitro/types.h>

typedef struct UnkStruct_0232e844 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0232e844;

void func_ov000_0232e7e0(void);

void func_ov000_0232e844(UnkStruct_0232e844 *s) {
    s->func = func_ov000_0232e7e0;
}
