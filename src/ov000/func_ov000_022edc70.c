#include <nitro/types.h>

typedef struct UnkStruct_022edc70 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1c4];
    u16 unk230;
} UnkStruct_022edc70;

void func_ov000_022edc24(void);

void func_ov000_022edc70(UnkStruct_022edc70 *s) {
    if (s->unk230++ > 0x3c)
        s->func = func_ov000_022edc24;
}
