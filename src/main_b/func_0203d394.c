#include <nitro/types.h>

typedef struct { u8 unk0[0x7c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8400]; UnkNode *listFree; void *listA; void *listB; u8 unk840C[0x18]; s32 count; } UnkStruct_0203d394;
void func_0203d3e8(void *from, void *to, UnkNode *n);
UnkNode *func_0203d394(UnkStruct_0203d394 *s) { UnkNode *n = s->listFree; func_0203d3e8(&s->listFree, &s->listB, n); n->unk7C = 1; s->count++; return n; }
