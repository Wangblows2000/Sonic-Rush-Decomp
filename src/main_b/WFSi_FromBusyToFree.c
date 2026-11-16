#include <nitro/types.h>

typedef struct { u8 unk0[0x7c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8400]; void *listFree; void *listA; void *listB; u8 unk840C[0x18]; s32 count; } UnkStruct_0203d2d0;
void WFSi_MoveList(void *from, void *to, UnkNode *n);
void WFSi_FromBusyToFree(UnkStruct_0203d2d0 *s, UnkNode *n) { WFSi_MoveList(&s->listB, &s->listFree, n); n->unk7C = 0; s->count--; }
