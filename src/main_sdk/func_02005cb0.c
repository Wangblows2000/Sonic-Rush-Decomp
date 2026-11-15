#include <nitro/types.h>

typedef struct { u8 pad[0x64]; u32 state; } UnkStruct_02005cb0;
BOOL func_02005cb0(UnkStruct_02005cb0 *p) { return p->state == 2; }
