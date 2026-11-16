#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
void func_0204bf54(void);
void func_0204b3b8(void);
void func_0204b414(void);
void VRAMTransfer_Clear(void);
void ScreenTransfer_Clear(void);
void PaletteTransfer_Clear(void);
void func_0204bbe4(void);
void func_0204bc4c(void) { func_0204bf54(); func_0204b3b8(); func_0204b414(); VRAMTransfer_Clear(); ScreenTransfer_Clear(); PaletteTransfer_Clear(); gTaskCurrent->fn = func_0204bbe4; }
