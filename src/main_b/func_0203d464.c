#include <nitro/types.h>

typedef struct UnkNode { struct UnkNode *next; s32 unk4; u8 unk8[0x74]; s32 unk7C; } UnkNode;
typedef struct { u8 unk0[0x8404]; UnkNode *head; } UnkStruct_0203d464;
UnkNode *func_0203d464(UnkStruct_0203d464 *s, s32 id) { UnkNode *n = s->head; if (n == NULL) return n; do { if (n->unk7C == 2 && n->unk4 == id) return n; n = n->next; } while (n != NULL); return n; }
