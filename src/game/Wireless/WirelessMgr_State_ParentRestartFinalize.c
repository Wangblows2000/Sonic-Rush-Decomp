#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u8 pad4[0x16];
    u16 unk1a;
} UnkObj_02042080;

extern u32 gSystemFlags;
extern u8 gWirelessSendBuffer[];
u32 WH_GetBitmap(void);
void WH_Finalize(void);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void WirelessMgr_State_ParentReconnect(void *);

void WirelessMgr_State_ParentRestartFinalize(UnkObj_02042080 *o) {
    if ((WH_GetBitmap() & ~1) == 0) {
        gSystemFlags &= ~0x100;
        gSystemFlags |= 0x200;
        WH_Finalize();
        {
            vu32 v = 0;
            MIi_CpuClear32(v, gWirelessSendBuffer, 4);
        }
        o->unk1a = 0;
        o->func = WirelessMgr_State_ParentReconnect;
    }
}
