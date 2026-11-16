#include <nitro/types.h>

typedef struct {
    u32 flags;
    void (*cb)(void);
} UnkWork_0204bc90;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204bc90 *work;
} UnkTask_0204bc90;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[4];
    u32 flags;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x24];
    u32 flags;
} UnkStruct_022c4590;

extern UnkTask_0204bc90 *gTaskCurrent;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4590 data_022c4590;
void func_0204bf54(void);
void func_0204ca68(void);
void func_0204c964(void);
void func_0204cb10(void);
void func_0204b3b8(void);
void func_0204b414(void);
void func_0204b528(void);
void func_0204b624(void);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void Sound_ResetHeap(void);
void VRAMTransfer_Clear(void);
void ScreenTransfer_Clear(void);
void PaletteTransfer_Clear(void);
void Task_Destroy(UnkTask_0204bc90 *t);
void Touch_Disable(void);

void func_0204bc90(void) {
    UnkWork_0204bc90 *w = gTaskCurrent->work;
    void (*cb)(void) = w->cb;
    u32 flags = w->flags;
    func_0204bf54();
    if (data_022c4560.unk10 == 0) {
        if (data_022c4560.flags & 0x40) {
            func_0204ca68();
            func_0204c964();
        } else {
            func_0204cb10();
        }
    }
    func_0204b3b8();
    func_0204b414();
    if (flags & 2)
        func_0204b528();
    if (flags & 1)
        func_0204b624();
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    Sound_ResetHeap();
    VRAMTransfer_Clear();
    ScreenTransfer_Clear();
    PaletteTransfer_Clear();
    Task_Destroy(gTaskCurrent);
    if (!(data_022c4560.flags & 0x40))
        data_022c4590.flags &= ~2;
    Touch_Disable();
    if (cb != NULL)
        cb();
}
