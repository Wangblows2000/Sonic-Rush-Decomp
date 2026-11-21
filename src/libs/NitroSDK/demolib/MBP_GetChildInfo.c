#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_022b89e8;
extern UnkStruct_022b89e8 sMbpState;
typedef struct { u8 unk0[0x1e]; } UnkElem;
extern UnkElem sMbpChildInfo[];
UnkElem *MBP_GetChildInfo(s32 i) { if (sMbpState.unk2 & (1 << i)) return &sMbpChildInfo[i - 1]; return NULL; }
