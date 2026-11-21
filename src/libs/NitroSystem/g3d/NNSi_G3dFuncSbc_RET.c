#include <nitro/types.h>

typedef struct UnkStruct_02024ca4 UnkStruct_02024ca4;
struct UnkStruct_02024ca4 { u32 unk0; u32 unk4; u32 unk8; u32 unkC; void (*unk10)(UnkStruct_02024ca4 *); };

void NNSi_G3dFuncSbc_RET(UnkStruct_02024ca4 *s) {
    if (s->unk10 != NULL) s->unk10(s);
    s->unk8 |= 0x20;
}
