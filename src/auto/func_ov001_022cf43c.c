/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4560[];
extern u8 gSaveData[];
s32 func_ov001_022ceef4();
s32 Vram_SetBankForBGExtPltt();
void func_ov001_022cf2a8();
s32 Task_Create();
extern u8 gTalkDemoId[];
s32 TalkDemo_Create();
s32 func_ov001_022cf4a0();

void func_ov001_022cf43c(void) {
    if (*(u32 *)data_022c4560 == 0) {
        s32 t0 = (*(u8 *)((u8 *)gSaveData + 0xf) & 0x7f);
        if (t0 != 0x7f) {
            func_ov001_022ceef4(0x7f, t0);
            Vram_SetBankForBGExtPltt(0);
            Task_Create((u32)func_ov001_022cf2a8, 0, 0, 0x1000, 0, 4);
            *(u16 *)gTalkDemoId = 0x37;
            TalkDemo_Create(gTalkDemoId, 0x37);
            return;
        }
    }
    func_ov001_022cf4a0(0);
}

#pragma thumb reset
