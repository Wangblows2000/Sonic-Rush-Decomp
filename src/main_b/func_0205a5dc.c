#include <nitro/types.h>

typedef struct { u8 unk0[0x48]; u16 unk48; } UnkStruct_02087d04;
extern UnkStruct_02087d04 gDisplaySub;
typedef struct { u8 unk0[0x488]; u32 flags; u8 unk48C[0x4c]; u16 unk4D8; } UnkStruct_0205a5dc;
void func_02004040(s32 a, u16 b, s32 c, s32 d, s32 e);
void func_0205a5dc(UnkStruct_0205a5dc *s) { func_02004040(0, s->unk4D8, 0x7fff, 0, 0); gDisplaySub.unk48 = 0; s->flags |= 0x80000000; }
