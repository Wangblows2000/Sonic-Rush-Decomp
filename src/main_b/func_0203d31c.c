#include <nitro/types.h>

typedef struct { u8 unk0[0x7c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8404]; void *listA; void *listB; } UnkStruct_0203d31c;
void func_0203d3e8(void *from, void *to, UnkNode *n);
void func_0203d31c(UnkStruct_0203d31c *s, UnkNode *n) { func_0203d3e8(&s->listA, &s->listB, n); n->unk7C = 3; }
