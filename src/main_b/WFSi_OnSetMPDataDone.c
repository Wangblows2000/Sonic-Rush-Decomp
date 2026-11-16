#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; } UnkStruct_022b8be0;
extern UnkStruct_022b8be0 data_022b8be0;
void WFSi_SendAck(u32 v);
void WFSi_SetMPData(void);
void WFSi_OnSetMPDataDone(void) { if (data_022b8be0.unkC == 0) return; data_022b8be0.unk4 = 1; WFSi_SendAck(1); WFSi_SetMPData(); }
