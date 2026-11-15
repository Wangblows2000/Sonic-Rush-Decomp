#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_022b89e8;
extern UnkStruct_022b89e8 data_022b89e8;
typedef struct { u8 unk0[0x1e]; } UnkElem;
extern UnkElem data_022b89f8[];
UnkElem *func_0203af64(s32 i) { if (data_022b89e8.unk2 & (1 << i)) return &data_022b89f8[i - 1]; return NULL; }
