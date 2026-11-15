#include <nitro/types.h>

typedef struct UnkStruct_022f58ac {
    u8 pad[0x14];
    s32 unk14;
    u8 pad1[0x50];
    void (*func)(void);
} UnkStruct_022f58ac;

void func_ov000_022f5878(void);

void func_ov000_022f58ac(UnkStruct_022f58ac *s) {
    if (s->unk14 > 0x25600)
        s->func = func_ov000_022f5878;
}
