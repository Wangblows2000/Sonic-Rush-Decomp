#include <nitro/types.h>

typedef struct UnkStruct_022dc188 {
    u8 pad[0x3ac];
    s32 unk3AC;
    u8 pad1[0x10];
    void (*func)(void);
    s32 unk3C4;
} UnkStruct_022dc188;

void func_ov000_022dc158(void);

void func_ov000_022dc188(UnkStruct_022dc188 *s) {
    s->func = func_ov000_022dc158;
    s->unk3C4 = 0;
    s->unk3AC = -16;
}
