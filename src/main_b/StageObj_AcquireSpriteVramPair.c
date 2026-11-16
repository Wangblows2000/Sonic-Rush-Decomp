#include <nitro/types.h>

typedef struct { u8 unk0[0x34]; void *unk34; } UnkStruct_02069e68;
void *ObjVramRef_Acquire(void *s, s32 a, void *b);
void StageObj_AcquireSpriteVramPair(UnkStruct_02069e68 *a, UnkStruct_02069e68 *b, void *p, u8 *q) { a->unk34 = ObjVramRef_Acquire(q, 0, p); b->unk34 = ObjVramRef_Acquire(q + 8, 1, p); }
