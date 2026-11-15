#include <nitro/types.h>

typedef struct { u8 unk0[0x24]; u32 unk24; } UnkStruct_02042760;
BOOL func_02042760(UnkStruct_02042760 *s) { return s->unk24 != 0; }
