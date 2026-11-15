#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; u16 unk1E; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void func_02048ba4(u16 a);
void func_02048e9c(void) { func_02048ba4(data_022c4560.unk1E); }
