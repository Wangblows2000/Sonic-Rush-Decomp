#include <nitro/types.h>

typedef struct UnkStruct_0230b11c {
    u8 pad0[8];
    u32 offset;
} UnkStruct_0230b11c;

extern char data_ov001_0231e6b0[];

s32 strncmp(const void *a, const void *b, u32 n);

BOOL SpecialStage_RelocateSsdHeader(UnkStruct_0230b11c *p) {
    if (strncmp(p, data_ov001_0231e6b0, 3) != 0)
        return FALSE;
    p->offset += (u32)p;
    return TRUE;
}
