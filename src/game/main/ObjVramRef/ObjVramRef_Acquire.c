#include <nitro/types.h>

typedef struct { void *unk0; u16 unk4; } UnkStruct_02068434;
void *ObjVram_Alloc(void *a, void *b);
void *ObjVramRef_Acquire(UnkStruct_02068434 *s, void *a, void *b) { if (s->unk0 == NULL && b != NULL) s->unk0 = ObjVram_Alloc(a, b); s->unk4++; return s->unk0; }
