#include <nitro/types.h>

typedef struct { u8 unk0[0x7c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8400]; void *listFree; void *listA; void *listB; u8 unk840C[0x18]; s32 count; } UnkStruct_0203d2d0;
void func_0203d3e8(void *from, void *to, UnkNode *n);
void func_0203d2d0(UnkStruct_0203d2d0 *s, UnkNode *n) { func_0203d3e8(&s->listB, &s->listFree, n); n->unk7C = 0; s->count--; }
