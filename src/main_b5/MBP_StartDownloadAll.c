#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unka;
    u16 unkc;
} UnkStruct_022b89e8;

extern UnkStruct_022b89e8 sMbpState;

s32 MBP_GetChildState(u32 n);
void MB_DisconnectChild(u32 n);
void MBP_ChangeState(s32 a);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);

void MBP_StartDownloadAll(void) {
    u16 i;
    u32 e;
    u16 mask;
    s32 bit;

    MBP_ChangeState(3);
    for (i = 1; i < 16; i++) {
        bit = 1 << i;
        if ((sMbpState.unk2 & bit) && MBP_GetChildState(i) == 1) {
            mask = ~bit;
            e = OS_DisableInterrupts();
            sMbpState.unk2 &= mask;
            sMbpState.unk4 &= mask;
            sMbpState.unk6 &= mask;
            sMbpState.unk8 &= mask;
            sMbpState.unka &= mask;
            sMbpState.unkc &= mask;
            OS_RestoreInterrupts(e);
            MB_DisconnectChild(i);
        }
    }
}
