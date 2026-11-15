#include <nitro/types.h>

typedef struct UnkStruct_022f11dc {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x38];
    void (*func)(void);
    u8 pad2[0x3ce];
    u16 unk43A;
} UnkStruct_022f11dc;

void func_ov000_022f11b4(void);

void func_ov000_022f11dc(UnkStruct_022f11dc *s) {
    s->unk2E = 0;
    s->unk43A = 0;
    s->func = func_ov000_022f11b4;
}
