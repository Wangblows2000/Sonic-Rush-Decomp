#include <nitro/types.h>

typedef struct { u8 unk0; u8 unk1; u8 unk2[0xe]; s32 unk10; s32 unk14; } UnkStruct_0206bd24;
void StageObj_Draw2DEx(s32 a, s32 b, u8 c, UnkStruct_0206bd24 *d, s32 e, s32 f, s32 g);
void StageObj_Draw2D(UnkStruct_0206bd24 *s, s32 e, s32 f) { StageObj_Draw2DEx(s->unk10, s->unk14, s->unk1, s, e, f, 0); }
