#include <nitro/types.h>

typedef struct UnkStruct_02334264 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02334264;

void func_ov000_023341d8(void);

void func_ov000_02334264(UnkStruct_02334264 *s) {
    s->func = func_ov000_023341d8;
}
