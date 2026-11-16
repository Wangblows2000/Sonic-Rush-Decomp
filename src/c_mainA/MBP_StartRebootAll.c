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

BOOL MB_CommResponseRequest(u32 n, s32 a);
void MB_DisconnectChild(u32 n);
void MBP_ChangeState(s32 a);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);

static inline void MBPi_DisconnectChild(u16 n)
{
    u32 e;
    u16 mask;

    mask = ~(1 << n);
    e = OS_DisableInterrupts();
    sMbpState.unk2 &= mask;
    sMbpState.unk4 &= mask;
    sMbpState.unk6 &= mask;
    sMbpState.unk8 &= mask;
    sMbpState.unka &= mask;
    sMbpState.unkc &= mask;
    OS_RestoreInterrupts(e);
    MB_DisconnectChild(n);
}

void MBP_StartRebootAll(void)
{
    u16 i;
    u16 busy;
    s32 bit;

    busy = 0;
    for (i = 1; i < 16; i++) {
        bit = 1 << i;
        if (sMbpState.unka & bit) {
            if (!MB_CommResponseRequest(i, 3)) {
                MBPi_DisconnectChild(i);
            } else {
                busy |= bit;
            }
        }
    }
    if (busy == 0)
        MBP_ChangeState(7);
    else
        MBP_ChangeState(4);
}
