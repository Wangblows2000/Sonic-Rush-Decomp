#include <nitro/types.h>

typedef struct { u8 unk0[8]; u32 unk8; } UnkStruct_022c4680;
extern UnkStruct_022c4680 gSaveData;
void SaveData_Unk_02059ba0(BOOL on) { if (on) gSaveData.unk8 &= ~1; else gSaveData.unk8 |= 1; }
