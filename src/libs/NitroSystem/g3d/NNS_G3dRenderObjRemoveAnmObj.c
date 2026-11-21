#include <nitro/types.h>

BOOL removeLink_(void *list, void *node);

typedef struct { u32 flags; u32 unk4; u8 list8[8]; u8 list10[8]; u8 list18[8]; } UnkStruct_02021c4c;

void NNS_G3dRenderObjRemoveAnmObj(UnkStruct_02021c4c *s, void *node) {
    if (removeLink_(s->list8, node) || removeLink_(s->list10, node) || removeLink_(s->list18, node)) {
        s->flags |= 0x10;
    }
}
