#include <nitro/types.h>

typedef struct { void *unk0; u16 unk4; u8 unk6[2]; void *unk8; u16 unkC; } UnkStruct_020682f0;
void Model_Release(void *a);
void func_020682f0(void *a, UnkStruct_020682f0 *s) { if (s->unk0 == NULL) return; if (s->unk8 == NULL) return; s->unk4--; s->unkC--; if (s->unk4 != 0) return; Model_Release(a); s->unk0 = NULL; s->unk8 = NULL; }
