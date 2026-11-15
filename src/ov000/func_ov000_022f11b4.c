#include <nitro/types.h>

typedef struct UnkStruct_022f11b4 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x3ce];
    u16 unk43A;
} UnkStruct_022f11b4;

void func_ov000_022f112c(void);

void func_ov000_022f11b4(UnkStruct_022f11b4 *s) {
    s->unk43A++;
    if (s->unk43A != 0)
        s->func = func_ov000_022f112c;
}
