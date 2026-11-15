#include <nitro/types.h>

typedef struct { u8 unk0[0x4c]; u8 unk4C; u8 unk4D; } UnkStruct_022c4490;
extern UnkStruct_022c4490 data_022c4490;
typedef struct { u32 unk0; u32 unk4; } UnkElem;
typedef struct { UnkElem arr[3]; } UnkRow;
extern UnkRow data_020745a8[];
s32 func_0204afc8(void *p, s32 a);
s32 func_0204b6cc(void) { return func_0204afc8(&data_020745a8[data_022c4490.unk4C].arr[data_022c4490.unk4D], 0); }
