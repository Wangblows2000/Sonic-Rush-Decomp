#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 b; u16 c; } UnkStruct_020183d4;
extern UnkStruct_020183d4 *data_02080f18;
u32 MBi_GetGgid(void) { return data_02080f18->b; }
