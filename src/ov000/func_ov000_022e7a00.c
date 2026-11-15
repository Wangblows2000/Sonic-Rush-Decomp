#include <nitro/types.h>

typedef struct UnkStruct_022e7a00 {
    u8 pad[0x440];
    void (*func)(void);
    u8 pad1[0x26];
    u16 unk46A;
} UnkStruct_022e7a00;

void func_ov000_022e79b8(void);

void func_ov000_022e7a00(UnkStruct_022e7a00 *s) {
    s->unk46A = 0;
    s->func = func_ov000_022e79b8;
}
