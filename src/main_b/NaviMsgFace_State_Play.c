#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; u8 unkD8[0xc]; s32 unkE4; } UnkStruct_02052d44;
void NaviMsgFace_StartIdle(UnkStruct_02052d44 *s);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsgFace_State_Play(UnkStruct_02052d44 *s) { if (!(s->flags & 0x100)) return; if (s->unkE4 <= 8) { NaviMsgFace_StartIdle(s); return; } NaviMsg_SetFaceAnim(10); }
