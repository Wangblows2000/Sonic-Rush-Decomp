#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *data_022b4574;
void func_0204d214(s32 a);
void func_0204ce7c(s32 a);
void func_0204b4cc(void);
void func_0204b470(void);
void func_0204bbbc(void);
void func_0204bbe4(void) { if (data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2) func_0204d214(1); else func_0204ce7c(1); func_0204b4cc(); func_0204b470(); data_022b4574->fn = func_0204bbbc; }
