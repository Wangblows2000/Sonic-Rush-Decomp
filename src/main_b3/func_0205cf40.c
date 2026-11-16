#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4b4];
    s32 unk4B4;
    s32 unk4B8;
    s32 unk4BC;
    s32 unk4C0;
    s32 unk4C4;
    s32 unk4C8;
} UnkWork_0205cf40;

typedef struct {
    void (*fn)(void);
} UnkObj_0205cf40;
typedef struct { u8 pad0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

void func_0205cf28(void);

void func_0205cf40(UnkWork_0205cf40 *w, UnkObj_0205cf40 *o) {
    if (gGameState.unk1C != 8 || gGameState.unk1D != 2) {
        w->unk4B4 = 0x9e000;
        w->unk4B8 = -0x5b000;
        w->unk4BC = 0x1e000;
        w->unk4C0 = 0x80000;
        w->unk4C4 = -0x5b000;
        w->unk4C8 = 0;
    } else {
        w->unk4B4 = 0x80000;
        w->unk4B8 = -0x5b000;
        w->unk4BC = 0x1e000;
        w->unk4C0 = 0x80000;
        w->unk4C4 = -0x5b000;
        w->unk4C8 = 0;
    }
    o->fn = func_0205cf28;
}
