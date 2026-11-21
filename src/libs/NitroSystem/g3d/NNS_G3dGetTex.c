#include <nitro/types.h>

/* Returns a block inside a resource file: for single-block files only when the
 * magic is 'BTX0' (NitroSystem texture). Matched (CW 1.2). Possibly NitroSystem. */
typedef struct { u32 magic; u32 unk4; u32 unk8; u16 headerSize; u16 numBlocks; } UnkResHeader_02025e8c;

u32 NNS_G3dGetTex(UnkResHeader_02025e8c *h) {
    u32 *blockOffsets = (u32 *)((u8 *)h + h->headerSize);
    if (h->numBlocks == 1) {
        if (h->magic == 0x30585442) return (u32)h + blockOffsets[0];
        return 0;
    }
    return (u32)h + blockOffsets[1];
}
