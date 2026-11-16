#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 mode;
    s32 state;
    u8 padc[0xe];
    u16 unk1a;
} UnkWork_0204138c;

extern u32 gSystemFlags;

void WFS_End(void);
void WH_Reset(void);
u32 WH_GetSystemState(void);
void WH_End(void);

void func_0204138c(UnkWork_0204138c *w) {
    BOOL loop;
    BOOL f;

    if (w->state != 7) {
        w->state = 7;
        gSystemFlags &= ~0x100;
        loop = TRUE;
        WFS_End();
        WH_Reset();
        f = FALSE;
        do {
            switch ((s32)WH_GetSystemState()) {
            case 1:
                WH_End();
            case 0:
            case 9:
            case 10:
                loop = f;
                break;
            }
        } while (loop);
    }
    w->unk1a = 0;
}
