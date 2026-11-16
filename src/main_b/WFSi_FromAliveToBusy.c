#include <nitro/types.h>

typedef struct { u8 unk0[0x7c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8404]; void *listA; void *listB; } UnkStruct_0203d31c;
void WFSi_MoveList(void *from, void *to, UnkNode *n);
void WFSi_FromAliveToBusy(UnkStruct_0203d31c *s, UnkNode *n) { WFSi_MoveList(&s->listA, &s->listB, n); n->unk7C = 3; }
