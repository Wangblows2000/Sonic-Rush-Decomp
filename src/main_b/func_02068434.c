#include <nitro/types.h>

typedef struct { void *unk0; u16 unk4; } UnkStruct_02068434;
void *func_02039c18(void *a, void *b);
void *func_02068434(UnkStruct_02068434 *s, void *a, void *b) { if (s->unk0 == NULL && b != NULL) s->unk0 = func_02039c18(a, b); s->unk4++; return s->unk0; }
