#include <nitro/types.h>

typedef struct UnkStruct_023141cc {
    u8 pad0[8];
    u32 offset;
} UnkStruct_023141cc;

extern char data_ov001_0231ef80[];

s32 strncmp(const void *a, const void *b, u32 n);

BOOL func_ov001_023141cc(UnkStruct_023141cc *p) {
    if (strncmp(p, data_ov001_0231ef80, 3) != 0)
        return FALSE;
    p->offset += (u32)p;
    return TRUE;
}
