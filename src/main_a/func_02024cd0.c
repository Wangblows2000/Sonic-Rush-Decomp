#include <nitro/types.h>

typedef struct UnkStruct_02024cd0 UnkStruct_02024cd0;
struct UnkStruct_02024cd0 { u32 unk0; u32 unk4; u32 unk8; void (*unkC)(UnkStruct_02024cd0 *); };

void func_02024cd0(UnkStruct_02024cd0 *s) {
    if (s->unkC != NULL) s->unkC(s);
    s->unk0++;
}
