#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; s32 unk10; u8 unk14[4]; u32 unk18; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
void func_0204b4cc(void);
void func_0204b470(void);
void func_0204c9b0(void);
void func_0204c8e8(void);
void func_0204cc48(void);
void func_0204bd6c(void);
void func_0204bd94(void) { func_0204b4cc(); func_0204b470(); if (data_022c4560.unk10 == 0) { if (data_022c4560.unk18 & 0x40) { func_0204c9b0(); func_0204c8e8(); } else func_0204cc48(); } gTaskCurrent->fn = func_0204bd6c; }
