#include <nitro/types.h>

typedef struct { u8 unk0[8]; void *unk8; } UnkC;
typedef struct { u8 unk0[8]; UnkC *unk8; } UnkB;
typedef struct { u8 unk0[0x10]; UnkB *unk10; } UnkTask;
extern UnkTask *gNaviMTask;
void NaviMPartA_State_Count(void);
BOOL NaviM_IsCounting(void) { return gNaviMTask->unk10->unk8->unk8 == (void *)NaviMPartA_State_Count; }
