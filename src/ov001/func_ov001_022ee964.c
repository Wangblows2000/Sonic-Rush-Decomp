#include <nitro/types.h>

typedef struct {
    s32 w0;
    u8 pad4[0xc];
    s32 w10;
    s32 w14;
    s32 w18;
    u8 b1c;
    u8 b1d;
    u16 h1e;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern s32 data_ov001_023167bc[][5];
u8 *ExtraBoss_GetWork(void);
void func_ov001_022f3530(void);
void func_ov001_022f6be8(void);

void func_ov001_022ee964(void) {
    u8 *g = ExtraBoss_GetWork();
    BOOL b = TRUE;
    s32 k;
    if (gGameState.w10 != 2 && gGameState.w10 != 3) {
        b = FALSE;
    }
    if (b) {
        k = 1;
    } else {
        k = gGameState.w14;
    }
    *(s32 *)(g + 0x30c) = data_ov001_023167bc[k][*(s32 *)(g + 0x40)];
    *(s32 *)(g + 0x308) = *(s32 *)(g + 0x30c);
    if (*(s32 *)(g + 0x40) == 1 || *(s32 *)(g + 0x40) == 3) {
        func_ov001_022f3530();
    } else {
        func_ov001_022f6be8();
    }
}
