#include <nitro/types.h>

extern s32 gFileRequestUsed;
extern u32 gFileRequestFreeList[];

u32 FileRequest_AllocNode(void) {
    s32 n = gFileRequestUsed;
    if (n >= 16) return 0;
    gFileRequestUsed = n + 1;
    return gFileRequestFreeList[n];
}
