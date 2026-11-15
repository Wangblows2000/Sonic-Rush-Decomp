#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
typedef struct { u8 unk0[0x474]; void (*fn)(void); u8 unk478[0x456]; u16 unk8CE; } UnkStruct_0205aa80;
void func_ov000_023375d8(void);
void func_0205a728(void);
void func_0205aa80(UnkStruct_0205aa80 *s) { if (data_022c4560.unk1C != 8 || data_022c4560.unk1D != 2) func_ov000_023375d8(); s->unk8CE = 0; s->fn = func_0205a728; }
