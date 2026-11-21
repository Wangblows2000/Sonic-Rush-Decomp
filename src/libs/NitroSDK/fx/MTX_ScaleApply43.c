#include <nitro/types.h>

typedef struct { u8 pad[0x24]; s32 x, y, z; } UnkStruct_02001bc0;
void MTX_ScaleApply33(const UnkStruct_02001bc0 *a, UnkStruct_02001bc0 *b, u32 c, u32 d, u32 e);
void MTX_ScaleApply43(const UnkStruct_02001bc0 *a, UnkStruct_02001bc0 *b, u32 c, u32 d, u32 e) { MTX_ScaleApply33(a, b, c, d, e); b->x = a->x; b->y = a->y; b->z = a->z; }
