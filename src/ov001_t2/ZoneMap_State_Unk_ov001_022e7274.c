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

extern UnkTask_022e7274 *gTaskCurrent;

void ZoneMap_Unk_ov001_022e6dec(UnkWork_022e7274 *w);
BOOL MsgBox_IsClosed(void);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
BOOL MsgBox_GetResult(void);
void ZoneMap_Unk_ov001_022e606c(void);
void ZoneMap_Unk_ov001_022e5f80(void);
void ZoneMap_Unk_ov001_022e6870(void);

#pragma thumb on
static inline u32 GetPlane_022e7274(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetPlane_022e7274(u32 plane) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (plane << 8);
}

void ZoneMap_State_Unk_ov001_022e7274(void) {
    UnkWork_022e7274 *w = gTaskCurrent->work;

    ZoneMap_Unk_ov001_022e6dec(w);
    w->timer++;
    if (MsgBox_IsClosed()) {
        w->timer = 0;
        w->sub->unk5c = 1;
        GX_SetGraphicsMode(1, 5, 1);
        SetPlane_022e7274(GetPlane_022e7274() & ~4);
        if (!MsgBox_GetResult()) {
            switch (w->mode) {
            case 0:
                ZoneMap_Unk_ov001_022e606c();
                break;
            case 1:
                ZoneMap_Unk_ov001_022e5f80();
                break;
            }
        } else {
            ZoneMap_Unk_ov001_022e6870();
        }
    }
}
#pragma thumb reset
