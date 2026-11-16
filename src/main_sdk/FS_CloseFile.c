#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 x8; u32 xc; u32 x10; } UnkStruct_0200bad8;
BOOL FSi_SendCommand(UnkStruct_0200bad8 *p, u32 a);
BOOL FS_CloseFile(UnkStruct_0200bad8 *p) { if (!FSi_SendCommand(p, 8)) return 0; p->x8 = 0; p->x10 = 0xe; p->xc &= ~0x30; return 1; }
