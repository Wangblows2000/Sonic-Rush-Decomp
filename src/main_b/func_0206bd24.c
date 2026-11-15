#include <nitro/types.h>

typedef struct { u8 unk0; u8 unk1; u8 unk2[0xe]; s32 unk10; s32 unk14; } UnkStruct_0206bd24;
void func_0206b768(s32 a, s32 b, u8 c, UnkStruct_0206bd24 *d, s32 e, s32 f, s32 g);
void func_0206bd24(UnkStruct_0206bd24 *s, s32 e, s32 f) { func_0206b768(s->unk10, s->unk14, s->unk1, s, e, f, 0); }
