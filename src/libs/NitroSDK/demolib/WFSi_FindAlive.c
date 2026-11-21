#include <nitro/types.h>

typedef struct UnkNode { struct UnkNode *next; u8 unk4[0x54]; s32 unk58; s32 unk5C; u8 unk60[0x1c]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8404]; UnkNode *head; } UnkStruct_0203d518;
UnkNode *WFSi_FindAlive(UnkStruct_0203d518 *s, s32 a, s32 len) { UnkNode *n = s->head; if (n == NULL) return n; do { if (n->unk7C == 2 && a == n->unk58 && len == n->unk5C - n->unk58) return n; n = n->next; } while (n != NULL); return n; }
