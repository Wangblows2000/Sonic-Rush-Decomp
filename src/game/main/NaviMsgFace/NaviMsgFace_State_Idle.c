#include <nitro/types.h>

typedef struct { u8 unk0[0xd8]; void (*fn)(void); u8 unkDC[0x1c]; s32 unkF8; } UnkStruct_02052dd4;
void NaviMsgFace_State_Blink(void);
void NaviMsg_StartFaceSeq(UnkStruct_02052dd4 *s, s32 a);
void NaviMsgFace_State_Idle(UnkStruct_02052dd4 *s) { s->unkF8--; if (s->unkF8 > 0) return; s->fn = NaviMsgFace_State_Blink; NaviMsg_StartFaceSeq(s, 1); }
