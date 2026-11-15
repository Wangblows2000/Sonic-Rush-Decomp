#include <nitro/types.h>

typedef struct UnkStruct_022d4234 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0xba8 - 0x6c];
    u8 unkBA8;
} UnkStruct_022d4234;

void func_ov000_022d41e4(void);

void func_ov000_022d4234(UnkStruct_022d4234 *s) {
    if (s->unkBA8 & 0x80)
        s->func = func_ov000_022d41e4;
}
