#include <nitro/types.h>

typedef struct { u8 pad[0x24]; s32 x, y, z; } UnkStruct_02001bc0;
void func_020013c8(const UnkStruct_02001bc0 *a, UnkStruct_02001bc0 *b, u32 c, u32 d, u32 e);
void func_02001bc0(const UnkStruct_02001bc0 *a, UnkStruct_02001bc0 *b, u32 c, u32 d, u32 e) { func_020013c8(a, b, c, d, e); b->x = a->x; b->y = a->y; b->z = a->z; }
