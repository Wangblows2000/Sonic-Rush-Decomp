#include <nitro/types.h>

typedef struct { u8 unk0[0x192a]; u16 unk192A; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
u32 func_02042958(void);
u32 WH_GetSystemState(void);
BOOL WH_EndScan(void);
void func_02043278(u16 v) { if (func_02042958() != 4) return; if (WH_GetSystemState() == 2) WH_EndScan(); data_022c4364->unk10->unk192A = v; }
