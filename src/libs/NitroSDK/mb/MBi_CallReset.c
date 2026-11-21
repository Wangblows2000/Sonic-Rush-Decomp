#include <nitro/types.h>

typedef struct { u8 pad[0x508]; u32 x508; } UnkStruct_020187cc;
extern UnkStruct_020187cc *data_02080f18;
s32 WM_Reset(u32 a);
void MBi_CheckWmErrcode(u32 a, s32 b);
s32 MBi_CallReset(void) { s32 r = WM_Reset(data_02080f18->x508); MBi_CheckWmErrcode(1, r); if (r == 2) r = 0; return r; }
