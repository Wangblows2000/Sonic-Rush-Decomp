#include <nitro/types.h>

BOOL func_02021cb4(void *list, void *node);

typedef struct { u32 flags; u32 unk4; u8 list8[8]; u8 list10[8]; u8 list18[8]; } UnkStruct_02021c4c;

void func_02021c4c(UnkStruct_02021c4c *s, void *node) {
    if (func_02021cb4(s->list8, node) || func_02021cb4(s->list10, node) || func_02021cb4(s->list18, node)) {
        s->flags |= 0x10;
    }
}
