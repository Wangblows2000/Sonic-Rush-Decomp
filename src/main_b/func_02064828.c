#include <nitro/types.h>

typedef struct { u8 unk0[0xe]; u16 unkE; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern u16 data_022c4db4;
extern u16 data_022c4db8;
u16 func_02064828(void) { u16 n = data_022c4560.unkE; if (data_022c4db4 < n && n >= 1) return data_022c4db8 = 1; return data_022c4db8 = 0; }
