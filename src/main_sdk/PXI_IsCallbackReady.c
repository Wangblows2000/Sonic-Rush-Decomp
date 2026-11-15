#include <nitro/types.h>

typedef struct { u8 pad[0x388]; u32 handlers[2]; } UnkStruct_0200a18c;
BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc) { UnkStruct_0200a18c *w = (UnkStruct_0200a18c *)0x27ffc00; return (w->handlers[proc] & (1 << fifotag)) ? 1 : 0; }
