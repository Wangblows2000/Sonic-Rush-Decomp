#include <nitro/types.h>

extern u8 gWirelessSendBuffer[];
extern u32 gSystemFlags;
extern void (*gWirelessErrorCallback)(void);
extern u16 gWHMaxChildCount;
extern u16 gWHPacketSize;
extern void *gWirelessRecvBuffers[];

s32 WH_GetSystemState(void);
u32 WH_GetBitmap(void);
s32 WH_GetErrorCode(void);
BOOL WH_StepDataSharing(void *buf);
BOOL WH_GetCurrentAid(void);
void *WH_GetSharedDataAdr(u16 aid);
void DC_StoreRange(const void *addr, u32 size);
void VBlankIntrWait(void);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void MI_CpuFill8(void *dst, u8 data, u32 size);

void Wireless_UpdateDataSharing(void) {
    u32 vcount;
    u16 i;
    if (WH_GetSystemState() != 5) {
        return;
    }
    vcount = WH_GetBitmap();
    DC_StoreRange(gWirelessSendBuffer, 0x200);
    if (gSystemFlags & 0x200) {
        WH_StepDataSharing(gWirelessSendBuffer);
    } else {
        while (!WH_StepDataSharing(gWirelessSendBuffer)) {
            if (WH_GetErrorCode() != 5) {
                if (gWirelessErrorCallback != NULL) {
                    gWirelessErrorCallback();
                }
                return;
            }
            VBlankIntrWait();
        }
        if (!WH_GetCurrentAid() && WH_GetBitmap() < vcount && gWirelessErrorCallback != NULL) {
            gWirelessErrorCallback();
        }
    }
    for (i = 0; i < gWHMaxChildCount + 1; i++) {
        void *src = WH_GetSharedDataAdr(i);
        if (src != NULL) {
            MI_CpuCopy8(src, gWirelessRecvBuffers[i], gWHPacketSize);
        } else {
            MI_CpuFill8(gWirelessRecvBuffers[i], 0, gWHPacketSize);
        }
    }
}
