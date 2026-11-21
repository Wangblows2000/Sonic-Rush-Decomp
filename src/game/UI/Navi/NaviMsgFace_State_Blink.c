#include <nitro/types.h>

typedef struct { u8 unk0[0xd4]; u32 flags; } UnkStruct_02052da8;
void NaviMsgFace_StartIdle(UnkStruct_02052da8 *s);
void NaviMsgFace_State_Blink(UnkStruct_02052da8 *s) { if (!(s->flags & 0x100)) return; NaviMsgFace_StartIdle(s); }
