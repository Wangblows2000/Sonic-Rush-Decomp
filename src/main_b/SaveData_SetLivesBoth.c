#include <nitro/types.h>

typedef struct { u8 unk0[0xe]; u8 unkE; u8 unkF[0x57]; u8 unk66; } UnkStruct_022c4680;
extern UnkStruct_022c4680 gSaveData;
void SaveData_SetLivesBoth(s32 a, s32 b) { if (a < 2) a = 2; else if (a > 99) a = 99; if (b < 2) b = 2; else if (b > 99) b = 99; gSaveData.unkE = a; gSaveData.unk66 = b; }
