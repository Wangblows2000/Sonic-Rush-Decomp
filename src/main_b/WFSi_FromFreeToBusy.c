#include <nitro/types.h>

typedef struct { u8 unk0[0x7c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8400]; UnkNode *listFree; void *listA; void *listB; u8 unk840C[0x18]; s32 count; } UnkStruct_0203d394;
void WFSi_MoveList(void *from, void *to, UnkNode *n);
UnkNode *WFSi_FromFreeToBusy(UnkStruct_0203d394 *s) { UnkNode *n = s->listFree; WFSi_MoveList(&s->listFree, &s->listB, n); n->unk7C = 1; s->count++; return n; }
