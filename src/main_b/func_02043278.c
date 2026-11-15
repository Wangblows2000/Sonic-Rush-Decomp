#include <nitro/types.h>

typedef struct { u8 unk0[0x192a]; u16 unk192A; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
u32 func_02042958(void);
u32 func_0203e934(void);
BOOL func_0203f560(void);
void func_02043278(u16 v) { if (func_02042958() != 4) return; if (func_0203e934() == 2) func_0203f560(); data_022c4364->unk10->unk192A = v; }
