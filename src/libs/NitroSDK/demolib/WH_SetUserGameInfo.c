#include <nitro/types.h>

typedef struct { u32 unk0; u16 unk4; u32 unk8; } UnkStruct_022c1a00;
extern UnkStruct_022c1a00 sParentParam;
void WH_SetUserGameInfo(u32 a, u16 b) { sParentParam.unk0 = a; sParentParam.unk4 = b; }
