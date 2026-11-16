#include <nitro/types.h>

typedef struct {
    u8 pad0[0x488];
    u32 unk488;
    u32 unk48C;
} UnkWork_0205cfd0;

typedef struct {
    u8 pad0[0x70];
    void (*unk70)(void);
    u8 pad74[0x24c - 0x74];
    s32 unk24C[5];
} UnkSub_0205cfd0;

typedef struct UnkObj_0205cfd0 {
    void (*fn)(UnkWork_0205cfd0 *w, struct UnkObj_0205cfd0 *o);
    UnkSub_0205cfd0 *sub;
} UnkObj_0205cfd0;
typedef struct { u8 pad0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

void func_0205d608(void);
void func_0205cf40(UnkWork_0205cfd0 *w, UnkObj_0205cfd0 *o);

void func_0205cfd0(UnkWork_0205cfd0 *w, UnkObj_0205cfd0 *o) {
    UnkSub_0205cfd0 *sub;
    u8 i;
    w->unk488 &= ~0x700000;
    w->unk48C &= ~0x400;
    if (gGameState.unk1C != 8 || gGameState.unk1D != 2) {
        sub = o->sub;
        for (i = 0; i < 5; i++) {
            sub->unk24C[i] = 0x199;
        }
    }
    o->sub->unk70 = func_0205d608;
    o->fn = func_0205cf40;
    o->fn(w, o);
}
