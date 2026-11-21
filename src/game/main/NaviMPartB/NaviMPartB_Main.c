#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void NaviMPartB_Draw(void *p);
void NaviMPartB_Main(void) { NaviMPartB_Draw(gTaskCurrent->unk10); }
