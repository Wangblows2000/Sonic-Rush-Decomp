#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_02321244_184;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x9C - 0x6C];
    u16 unk9C;
    u8 pad9E[0x184 - 0x9E];
    UnkStruct_02321244_184 *unk184;
} UnkStruct_02321244;

extern s16 data_0206e470[];

void func_02069818(void *w, s32 a);
u16 func_020027a4(s32 y, s32 x);
void func_ov000_02321188(void);

void func_ov000_02321244(UnkStruct_02321244 *w, UnkStruct_02321244 *o) {
    s32 idx;

    if (w->unk9C & 1) {
        func_02069818(w, (u16)(w->unk9C - 1));
    }
    w->unk68 = func_ov000_02321188;
    w->unk5C |= 4;
    w->unk54 |= 2;
    w->unk58 &= ~0x80;
    idx = (s32)(func_020027a4(w->unk14 - o->unk14, w->unk10 - o->unk10) & 0xffff) >> 4;
    w->unk2E = (data_0206e470[(u32)((idx << 1) + 1)] * 0x400) >> 12;
    w->unk30 = (data_0206e470[(u32)(idx << 1)] * 0x400) >> 12;
    if (w->unk2E > 0) {
        w->unk5C &= ~1;
    } else {
        w->unk5C |= 1;
    }
    w->unkC = 0xe;
    if (w->unk184->unk2 >= 0xab && w->unk184->unk2 < 0xad) {
        w->unk2E >>= 1;
        w->unk30 >>= 1;
    }
}
