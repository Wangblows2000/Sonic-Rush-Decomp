#include <nitro/types.h>

typedef struct { u8 unk0[0x7c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8404]; void *listA; void *listB; } UnkStruct_0203d358;
void WFSi_MoveList(void *from, void *to, UnkNode *n);
void WFSi_FromBusyToAlive(UnkStruct_0203d358 *s, UnkNode *n) { WFSi_MoveList(&s->listB, &s->listA, n); n->unk7C = 2; }
