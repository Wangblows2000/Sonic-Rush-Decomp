#include <nitro/types.h>

typedef struct { u8 unk0[8]; void *unk8; } UnkC;
typedef struct { u8 unk0[8]; UnkC *unk8; } UnkB;
typedef struct { u8 unk0[0x10]; UnkB *unk10; } UnkTask;
extern UnkTask *gNaviMTask;
void NaviM_Unk_02063d34(void);
BOOL NaviM_Unk_02064758(void) { return gNaviMTask->unk10->unk8->unk8 == (void *)NaviM_Unk_02063d34; }
