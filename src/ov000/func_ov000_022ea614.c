#include <nitro/types.h>

typedef struct UnkStruct_022ea614 {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x14];
    u16 unk454;
} UnkStruct_022ea614;

void func_ov000_022ea584(void);

BOOL func_ov000_022ea614(UnkStruct_022ea614 *s) {
    s->func = func_ov000_022ea584;
    s->unk454 = 0;
    s->func = func_ov000_022ea584;
    return TRUE;
}
