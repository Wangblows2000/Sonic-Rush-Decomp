#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[8];
    s32 unkc;
} UnkWork_02041300;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02041300 *work;
} UnkTask_02041300;

extern u32 gSystemFlags;
extern UnkTask_02041300 *data_022c4364;

void WH_Reset(void);
s32 WH_GetSystemState(void);
void WH_End(void);
void func_0204138c(void);

void func_02041300(void) {
    BOOL loop = TRUE;

    gSystemFlags &= 0x100;
    WH_Reset();
    do {
        switch (WH_GetSystemState()) {
        case 1:
            WH_End();
        case 9:
        case 10:
            loop = FALSE;
            break;
        }
    } while (loop);
    if (data_022c4364 != NULL) {
        UnkWork_02041300 *w = data_022c4364->work;
        w->unkc = 2;
        w->fn = func_0204138c;
    }
}
