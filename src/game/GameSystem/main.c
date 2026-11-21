#include <nitro/types.h>

typedef struct {
    u16 held;
} UnkPad_022503e0;

typedef struct {
    u8 pad0[0x80];
    u32 swapMode;   /* 0x80 */
    u32 swapDepth;  /* 0x84 */
} UnkStruct_0208f754;

extern u8 data_027e0000[];
extern UnkPad_022503e0 gPad;
extern u32 gSystemFlags;
extern u32 data_02087cc4;
extern u32 gSystemResetParameter;
extern UnkStruct_0208f754 data_0208f754;

void System_Init(void);
void Game_Init(void);
BOOL CARD_IsPulledOut(void);
void System_OnCardPulledOut(void);
void CARD_TerminateForPulledOut(void);
void Pad_Update(UnkPad_022503e0 *pad, u16 keys);
void Touch_Update(void);
void Mic_Update(void);
void System_SoftReset(u32 arg);
void System_HandleLidClose(void);
void System_Update3DErrorFlags(void);
void Overlay_Update(void);
void Task_RunMain(void);
void FileRequest_Update(void);
void Oam_Flush(u32 screen);
void VBlankIntrWait(void);
void Wireless_UpdateDataSharing(void);
void Display_VBlankUpdate(void);

#define REG_KEYINPUT   (*(vu16 *)0x04000130)
#define HW_BUTTON_XY   (*(vu16 *)0x027fffa8)
#define REG_POWCNT     (*(vu16 *)0x04000304)
#define REG_SWAP_BUFFERS (*(vu32 *)0x04000540)
#define HW_INTR_CHECK  (*(vu32 *)((u32)data_027e0000 + 0x3ff8))

static inline u32 GetPower(void)
{
    return REG_POWCNT & 0x20e;
}

void main(void)
{
    System_Init();
    Game_Init();
    for (;;) {
        if (CARD_IsPulledOut()) {
            System_OnCardPulledOut();
            CARD_TerminateForPulledOut();
        }
        if (!(gSystemFlags & 0x40)) {
            u16 keys = (u16)(((REG_KEYINPUT | HW_BUTTON_XY) ^ 0x2fff) & 0x2fff);
            Pad_Update(&gPad, keys);
        }
        Touch_Update();
        Mic_Update();
        if (!(gSystemFlags & 0x10) && (gPad.held & 0x30c) == 0x30c) {
            System_SoftReset(gSystemResetParameter);
        }
        System_HandleLidClose();
        System_Update3DErrorFlags();
        Overlay_Update();
        Task_RunMain();
        FileRequest_Update();
        Oam_Flush(0);
        Oam_Flush(1);
        if ((GetPower() & 0xc) && !(gSystemFlags & 0x8000000)) {
            REG_SWAP_BUFFERS = (data_0208f754.swapDepth << 1) | data_0208f754.swapMode;
        }
        data_02087cc4 = gSystemFlags;
        HW_INTR_CHECK &= ~1;
        VBlankIntrWait();
        gSystemFlags = data_02087cc4;
        if (gSystemFlags & 0x100) {
            Wireless_UpdateDataSharing();
        }
        Display_VBlankUpdate();
        gSystemFlags &= ~0x80000000;
        if (gSystemFlags & 8) {
            gSystemFlags |= 0x80000000;
        }
    }
}
