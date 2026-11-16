#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 b; u16 c; } UnkStruct_020183c0;
extern UnkStruct_020183c0 *data_02080f18;
u16 MBi_GetTgid(void) { return data_02080f18->c; }
