#include <nitro/types.h>

typedef struct { u8 pad[0x48]; void *a; u32 b; } UnkStruct_0200aee8;
void FS_SetArchiveProc(UnkStruct_0200aee8 *p, void *a, u32 b) { if (b == 0) a = 0; else if (a == 0) b = 0; p->a = a; p->b = b; }
