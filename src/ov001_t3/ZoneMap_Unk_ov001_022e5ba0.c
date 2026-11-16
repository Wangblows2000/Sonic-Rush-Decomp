#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5c];
    s32 unk5c;
} UnkSub_022e5ba0;

typedef struct {
    u8 pad0[0x74];
    s32 unk74;
    u8 pad78[0xa4 - 0x78];
    UnkSub_022e5ba0 *sub;
} UnkWork_022e5ba0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e5ba0 *work;
} UnkTask_022e5ba0;

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u16 unka;
} UnkStruct_0208922c;

extern UnkTask_022e5ba0 *gTaskCurrent;
extern UnkStruct_0208922c gDisplayMain;

void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void MsgBox_SetFlags(s32 a);
void ZoneMap_State_Unk_ov001_022e7274(void);

#pragma thumb on
static inline u32 GetPlane_022e5ba0(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetPlane_022e5ba0(u32 plane) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (plane << 8);
}

void ZoneMap_Unk_ov001_022e5ba0(void) {
    UnkWork_022e5ba0 *w;
    vu32 zero;

    w = gTaskCurrent->work;
    w->unk74 = 0;
    w->sub->unk5c = 1;
    SetPlane_022e5ba0(GetPlane_022e5ba0() & ~4);
    GX_SetGraphicsMode(1, 3, 1);
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x484;
    zero = 0;
    MIi_CpuClear32(zero, (void *)0x6002000, 0x800);
    gDisplayMain.unk8 = 0;
    gDisplayMain.unka = 0;
    MsgBox_Create(0x12, 3, 1, 0, 2, 1, 0xe, 0x100010);
    MsgBox_SetFlags(1);
    gTaskCurrent->func = ZoneMap_State_Unk_ov001_022e7274;
}
#pragma thumb reset
