#include <nitro/types.h>

typedef struct { u8 unk0; u8 unk1; u8 unk2[0xe]; s32 unk10; s32 unk14; s32 unk18; } UnkStruct_0206aa04;
void StageObj_Draw3DEx(s32 a, s32 b, s32 c, u8 d, UnkStruct_0206aa04 *e, s32 f);
void StageObj_Draw3D(UnkStruct_0206aa04 *s, s32 f) { StageObj_Draw3DEx(s->unk10, s->unk14, s->unk18, s->unk1, s, f); }
