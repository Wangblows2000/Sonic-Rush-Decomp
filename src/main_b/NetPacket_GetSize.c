#include <nitro/types.h>

typedef struct { u16 lo:10; u16 hi:6; } UnkStruct_0206c7f0;
extern u8 *data_022c4f18;
s32 NetPacket_GetSize(UnkStruct_0206c7f0 *p) { s32 r; if (data_022c4f18 == NULL) return p->hi; r = data_022c4f18[*(u8 *)p]; if (r == 0) r = 4; return r; }
