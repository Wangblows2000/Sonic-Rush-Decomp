#include <nitro/types.h>

typedef struct {
    u8 pad0[0x488];
    u32 unk488;
    u32 unk48C;
} UnkWork_0205bba4;

typedef struct UnkObj_0205bba4 {
    void (*fn)(UnkWork_0205bba4 *w, struct UnkObj_0205bba4 *o);
    u8 pad4[0x90];
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    u8 padA4[0x10];
    s32 unkB4;
    s32 unkB8;
    u8 padBC[0x4];
    s16 t;  /* 0xc0 */
} UnkObj_0205bba4;
typedef struct { u8 pad0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

void func_0205bb7c(UnkWork_0205bba4 *w, UnkObj_0205bba4 *o);

void func_0205bba4(UnkWork_0205bba4 *w, UnkObj_0205bba4 *o) {
    w->unk488 &= ~0x1000;
    w->unk48C |= 0x40;
    if (data_022c4560.unk1C < 7 && data_022c4560.unk1D < 2) {
        o->unk94 = -0x1c000;
        o->unk98 = 0;
        o->unk9C = 0;
        o->unkA0 = 0;
    } else {
        o->unk94 = -0x1c000;
        o->unk98 = 0;
        o->unk9C = -0x8000;
        o->unkA0 = 0;
    }
    o->unkB4 = o->unk94 * 2;
    o->unkB8 = o->unk98;
    o->t = 0;
    o->fn = func_0205bb7c;
    o->fn(w, o);
}
