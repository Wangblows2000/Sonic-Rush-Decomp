#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern void (*data_020741f0[][3])(void);
extern void *data_022c4484;
void func_ov000_0233bc40(void);
void func_ov000_02335954(void);
void func_02036820(void *p);
void func_0204c748(s32 a);
void func_0204b470(void) { func_ov000_0233bc40(); func_ov000_02335954(); func_02036820(data_022c4484); func_0204c748(0); data_020741f0[data_022c4560.unk1C][data_022c4560.unk1D](); }
