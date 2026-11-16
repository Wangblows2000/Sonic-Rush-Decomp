#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void NaviMPartA_Draw(void *p);
void NaviMPartA_Main(void) { NaviMPartA_Draw(gTaskCurrent->unk10); }
