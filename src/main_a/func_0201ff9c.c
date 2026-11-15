#include <nitro/types.h>

typedef struct { u8 unk0[0x10C]; u32 unk10C; u8 unk110[0x38]; u32 *unk148; } UnkStruct_0201ff9c;

void func_0201ff9c(UnkStruct_0201ff9c *s) {
    u32 *p = s->unk148;
    if (p != NULL) {
        *p = 0;
        s->unk148 = NULL;
    }
    s->unk10C &= ~1;
    s->unk10C &= ~4;
    s->unk10C &= ~2;
}
