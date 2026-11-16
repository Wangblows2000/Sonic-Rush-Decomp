#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    u16 unk62;
    u8 pad64[0x68 - 0x64];
    void (*unk68)(void);
    u8 pad6C[0x2C4 - 0x6C];
    u32 unk2C4;
    u8 pad2C8[0x324 - 0x2C8];
    u8 unk324;
    u8 pad325[3];
    s16 unk328;
    u8 pad32A[2];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x350 - 0x334];
    s16 unk350;
    u8 pad352[0x376 - 0x352];
    s16 unk376;
    u8 pad378[0x37C - 0x378];
    s16 unk37C;
    u8 pad37E[0x384 - 0x37E];
    u8 unk384[0x3AE - 0x384];
    s16 unk3AE;
    u8 pad3B0[2];
    s16 unk3B2;
    u8 pad3B4[0x3C4 - 0x3B4];
    s16 unk3C4;
} UnkStruct_023492f0_Player;

typedef struct {
    u8 pad0[0x18];
    UnkStruct_023492f0_Player *unk18;
} UnkStruct_023492f0;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_023492f0_Arg;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

void func_0204b7ec(void *p, s32 a, s32 b);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void func_ov000_0234307c(UnkStruct_023492f0_Player *w, s32 a);
void func_ov000_02336050(u32 a, s32 b);
void func_ov000_0235aea8(UnkStruct_023492f0_Player *w);
void func_ov000_02349038(UnkStruct_023492f0_Player *w);
void func_ov000_0234cb70(UnkStruct_023492f0_Player *w);
void func_ov000_0234c4a4(UnkStruct_023492f0_Player *w, s32 a);
void func_020446b0(s32 a);
void func_ov000_02349294(void);

#define ABS_023492f0(x) ((x) < 0 ? -(x) : (x))

void func_ov000_023492f0(UnkStruct_023492f0 *o, UnkStruct_023492f0_Arg *arg) {
    UnkStruct_023492f0_Player *w = o->unk18;
    s32 v;
    u32 old;

    if (w->unk3C4 != 0) {
        return;
    }
    if (w->unk62 & 0xf) {
        return;
    }
    v = (w->unk58 & 0x8000) ? w->unk2E : w->unk34;
    if (arg->unk20 & 0x40) {
        func_0204b7ec(w->unk384, 0, 0x43);
    }
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        func_020537b4(0xc);
        func_020533e0(0x11, 0x96);
    }
    if (!(w->unk32C & 0x10000000)) {
        func_ov000_0234307c(w, -0x4b0);
        func_ov000_02336050((u32)(w->unk350 << 12) >> 16, 1);
    }
    if (arg->unk20 & 0x20) {
        if (w->unk3AE != 0) {
            func_0204b7ec(w->unk384, 0, 5);
            func_ov000_0235aea8(w);
        }
        func_ov000_02349038(w);
        return;
    }
    if (!(w->unk32C & 0x10000000) && w->unk3AE == 0) {
        func_ov000_02349038(w);
        return;
    }
    old = w->unk330;
    func_ov000_0234cb70(w);
    w->unk330 |= old & 0x40000;
    if (ABS_023492f0(v) >= w->unk37C) {
        func_ov000_0234c4a4(w, 0x10);
    } else {
        func_ov000_0234c4a4(w, 0xf);
    }
    if (!(w->unk32C & 0x10000000)) {
        func_0204b7ec(w->unk384, 0, 5);
        func_ov000_0235aea8(w);
    }
    w->unk32C &= ~0x30000000;
    func_020446b0(3);
    w->unk3B2 = w->unk376;
    w->unk2C4 |= 5;
    if (ABS_023492f0(v) < 0x180) {
        v = 0;
    }
    w->unk2E = 0x180;
    w->unk30 = -0x300;
    w->unk34 = 0;
    if (v >= 0) {
        if (v > 0) {
            w->unk2E = -w->unk2E;
        } else if (!(w->unk5C & 1)) {
            w->unk2E = -w->unk2E;
        }
    }
    if (w->unk328 == 0x10) {
        w->unk2E = v >> 1;
    }
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk58 &= ~1;
    w->unk68 = func_ov000_02349294;
    if (w->unk324 != 1) {
        func_0204b7ec(w->unk384, 0, 0x21);
    } else {
        func_0204b7ec(w->unk384, 0, 0x2a);
    }
}
