#include <nitro/types.h>

void Heap_Free(u32 a);

typedef struct { u32 unk0; u32 *unk4; } UnkStruct_0203498c;

void Heap_AllocatorFree(UnkStruct_0203498c *s) {
    Heap_Free(*s->unk4);
}
