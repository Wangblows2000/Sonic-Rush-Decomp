#include <nitro/types.h>

typedef struct { u8 unk0[0x94]; u16 unk94; u8 unk96[0x802]; u16 unk898; } UnkStruct_02039bb0;
extern UnkStruct_02039bb0 *gDisplayTable[];
extern u32 gObjVramBaseTable[];
u32 OS_GetArenaLo(s32 a);
void ObjVram_Free(s32 i, u32 addr) { UnkStruct_02039bb0 *p = gDisplayTable[i]; if (addr == OS_GetArenaLo(0)) return; ((u16 *)((u8 *)p + 0x98))[((addr - gObjVramBaseTable[i]) >> (p->unk94 + 5)) - p->unk898] = 0; }
