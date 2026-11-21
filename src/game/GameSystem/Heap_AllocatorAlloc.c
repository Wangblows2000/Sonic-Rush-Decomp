#include <nitro/types.h>

void Heap_Alloc(u32 a, u32 b, u32 c);

typedef struct { u32 unk0; u32 *unk4; u32 unk8; } UnkStruct_020349a0;

void Heap_AllocatorAlloc(UnkStruct_020349a0 *s, u32 b) {
    Heap_Alloc(*s->unk4, b, s->unk8);
}
