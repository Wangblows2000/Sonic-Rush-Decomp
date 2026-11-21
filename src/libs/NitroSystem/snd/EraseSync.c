#include <nitro/types.h>

u32 SND_GetCurrentCommandTag(void);
void SND_FlushCommand(u32 a);
void SND_WaitForCommandProc(u32 a);

void EraseSync(void) {
    u32 x = SND_GetCurrentCommandTag();
    SND_FlushCommand(1);
    SND_WaitForCommandProc(x);
}
