#include <nitro/types.h>

typedef struct { u8 unk0[0xd8]; void (*fn)(void); } UnkStruct_02052d90;
void NaviMsgFace_State_Play(void);
void NaviMsg_StartFaceSeq(UnkStruct_02052d90 *s, void *a);
void NaviMsgFace_Play(UnkStruct_02052d90 *s, void *a) { s->fn = NaviMsgFace_State_Play; NaviMsg_StartFaceSeq(s, a); }
