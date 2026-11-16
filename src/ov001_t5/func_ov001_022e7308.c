#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5c];
    s32 unk5c;
} UnkSub_022e7308;

typedef struct {
    u8 pad0[0x74];
    s32 timer;
    u8 pad78[0xa4 - 0x78];
    UnkSub_022e7308 *sub;
} UnkWork_022e7308;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e7308 *work;
} UnkTask_022e7308;

extern UnkTask_022e7308 *gTaskCurrent;

void func_ov001_022e6dec(UnkWork_022e7308 *w);
BOOL func_02050018(void);
void func_02002ab0(s32 a, s32 b, s32 c);
void func_ov001_022e6870(void);

#define REG_DISPCNT_022e7308 (*(vu32 *)0x4000000)

static inline u32 GetVisiblePlane_022e7308(void) {
    return (REG_DISPCNT_022e7308 & 0x1f00) >> 8;
}

static inline void SetVisiblePlane_022e7308(u32 plane) {
    REG_DISPCNT_022e7308 = (REG_DISPCNT_022e7308 & ~0x1f00) | (plane << 8);
}

#pragma thumb on
void func_ov001_022e7308(void) {
    UnkWork_022e7308 *w;

    w = gTaskCurrent->work;
    func_ov001_022e6dec(w);
    w->timer++;
    if (func_02050018()) {
        w->timer = 0;
        w->sub->unk5c = 1;
        func_02002ab0(1, 5, 1);
        SetVisiblePlane_022e7308(GetVisiblePlane_022e7308() & ~4);
        func_ov001_022e6870();
    }
}
#pragma thumb reset
