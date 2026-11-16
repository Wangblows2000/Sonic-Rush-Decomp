#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5c];
    s32 unk5c;
} UnkSub_022e7274;

typedef struct {
    s32 mode;
    u8 pad4[0x74 - 4];
    s32 timer;
    u8 pad78[0xa4 - 0x78];
    UnkSub_022e7274 *sub;
} UnkWork_022e7274;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e7274 *work;
} UnkTask_022e7274;

extern UnkTask_022e7274 *data_022b4574;

void func_ov001_022e6dec(UnkWork_022e7274 *w);
BOOL func_02050018(void);
void func_02002ab0(s32 a, s32 b, s32 c);
BOOL func_0205000c(void);
void func_ov001_022e606c(void);
void func_ov001_022e5f80(void);
void func_ov001_022e6870(void);

#pragma thumb on
static inline u32 GetPlane_022e7274(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetPlane_022e7274(u32 plane) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (plane << 8);
}

void func_ov001_022e7274(void) {
    UnkWork_022e7274 *w = data_022b4574->work;

    func_ov001_022e6dec(w);
    w->timer++;
    if (func_02050018()) {
        w->timer = 0;
        w->sub->unk5c = 1;
        func_02002ab0(1, 5, 1);
        SetPlane_022e7274(GetPlane_022e7274() & ~4);
        if (!func_0205000c()) {
            switch (w->mode) {
            case 0:
                func_ov001_022e606c();
                break;
            case 1:
                func_ov001_022e5f80();
                break;
            }
        } else {
            func_ov001_022e6870();
        }
    }
}
#pragma thumb reset
