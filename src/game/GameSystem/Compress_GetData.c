#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; } UnkStruct_02033a54;

u32 *Compress_GetData(UnkStruct_02033a54 *s) {
    return &s->unk4;
}
