#include <nitro/types.h>

typedef struct { u8 unk0[0xd8]; void (*fn)(void); u8 unkDC[0x1c]; s32 unkF8; } UnkStruct_02052e1c;
extern u32 gRandSeed;
void NaviMsgFace_State_Idle(void);
void NaviMsg_StartFaceSeq(UnkStruct_02052e1c *s, s32 a);
void NaviMsgFace_StartIdle(UnkStruct_02052e1c *s) { s->fn = NaviMsgFace_State_Idle; NaviMsg_StartFaceSeq(s, 0); gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f; s->unkF8 = ((u16)(gRandSeed >> 16) & 0x3f) + 0x2d0; }
