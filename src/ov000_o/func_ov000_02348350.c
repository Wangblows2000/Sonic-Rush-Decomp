#include <nitro/types.h>

typedef struct UnkStruct_02348350 UnkStruct_02348350;

struct UnkStruct_02348350 {
    u8 pad0;
    u8 unk1;
    u8 pad2[8 - 2];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x34 - 0x10];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u16 unk5E;
    u8 pad60[0x324 - 0x60];
    u8 unk324;
    u8 pad325[0x328 - 0x325];
    s16 unk328;
    s16 unk32A;
    u8 pad32C[0x334 - 0x32C];
    void (*unk334)(UnkStruct_02348350 *w);
    u8 pad338[0x340 - 0x338];
    void (*unk340)(UnkStruct_02348350 *w);
    u8 pad344[0x352 - 0x344];
    s16 unk352;
    u8 pad354[0x384 - 0x354];
    u8 unk384[4];
    u8 unk388[8];
    u8 unk390[0x410 - 0x390];
    u8 unk410;
    u8 pad411[0x420 - 0x411];
    u16 unk420;
    u16 unk422;
};

extern s16 data_0206e470[];

#define SIN_02348350(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

void func_0201c070(void *p, s32 a);
void func_0201bf1c(void *p);
void func_0204b7ec(void *p, s32 a, s32 b);
void func_ov000_023558bc(UnkStruct_02348350 *w, s32 a, s32 b);
BOOL func_ov000_023470ec(UnkStruct_02348350 *w);
void func_ov000_0234ab34(UnkStruct_02348350 *w);
void func_ov000_023497a4(UnkStruct_02348350 *w);
void func_ov000_0234c4a4(UnkStruct_02348350 *w, s32 state);
void func_ov000_0234801c(UnkStruct_02348350 *w);

void func_ov000_02348350(UnkStruct_02348350 *w) {
    u8 ang = w->unk1;
    s32 d;

    if (w->unk58 & 4) {
        w->unk5C ^= 1;
    }
    if (w->unk34 == 0 && (w->unk1 == 0 || w->unk1 == 0x80)) {
        w->unk34 = 0x100;
        if (w->unk5C & 1) {
            w->unk34 = -w->unk34;
        }
    }
    if (!(w->unk58 & 1)) {
        func_0201c070(w->unk388, 0);
        func_0201c070(w->unk390, 0x20);
        func_0201bf1c(w->unk390);
        w->unk54 |= 1;
        w->unk410 = 0;
        func_ov000_023558bc(w, w->unk34, -0x500);
        if (w->unk324 != 1) {
            func_0204b7ec(w->unk384, 0, 2);
            return;
        }
        func_0204b7ec(w->unk384, 0, 0x13);
        return;
    }
    w->unk1 = 0;
    if (func_ov000_023470ec(w)) {
        func_0201c070(w->unk388, 0);
        func_0201c070(w->unk390, 0x20);
        func_0201bf1c(w->unk390);
        w->unk54 |= 1;
        w->unk410 = 0;
        func_ov000_0234ab34(w);
        return;
    }
    w->unk1 = ang;
    if (w->unk422 & 3) {
        func_0201c070(w->unk388, 0);
        func_0201c070(w->unk390, 0x20);
        func_0201bf1c(w->unk390);
        if (w->unk5E & 0x10) {
            w->unk54 |= 1;
            w->unk410 = 0;
            func_ov000_023558bc(w, w->unk34, -0x700);
            func_ov000_023497a4(w);
            return;
        }
        if (w->unk340 != NULL) {
            w->unk54 |= 1;
            w->unk340(w);
            return;
        }
    }
    if (!(w->unk5E & 8) && w->unk334 != NULL) {
        func_0201c070(w->unk388, 0);
        func_0201c070(w->unk390, 0x20);
        func_0201bf1c(w->unk390);
        w->unk54 |= 1;
        w->unk410 = 0;
        w->unk334(w);
        return;
    }
    d = ((SIN_02348350((u16)(w->unk1 << 8)) * 0x28) << 4) >> 16;
    if ((w->unk5C & 1) && d > 0) {
        d = (s16)(d >> 2);
        if (w->unk352 != 0) {
            d = 0;
        }
    }
    if (!(w->unk5C & 1) && d < 0) {
        d = (s16)(d >> 2);
        if (w->unk352 != 0) {
            d = 0;
        }
    }
    w->unk34 += d;
    if (w->unk34 > 0) {
        w->unk5C &= ~1;
    } else if (w->unk34 < 0) {
        w->unk5C |= 1;
    }
    if (w->unk420 & 0x20) {
        if (w->unk34 < 0 || w->unk34 > 0x200) {
            w->unk34 -= 0x10;
        }
    } else if (w->unk420 & 0x10) {
        if (w->unk34 > 0 || w->unk34 < -0x200) {
            w->unk34 += 0x10;
        }
    }
    switch (w->unk328) {
    case 0x1e:
        if (w->unk5C & 8) {
            func_ov000_0234c4a4(w, 0x1f);
            w->unk5C |= 4;
        }
        break;
    case 0x1f:
        w->unk8++;
        if (w->unk8 >= 0x20 && (w->unk5C & 8)) {
            func_ov000_0234c4a4(w, 0x20);
            w->unk8 = 0;
        }
        break;
    case 0x20:
        if (w->unk5C & 8) {
            func_0201c070(w->unk390, 0x20);
            func_0201bf1c(w->unk390);
            func_ov000_0234c4a4(w, 0x1a);
            w->unk5C |= 4;
        }
        break;
    }
    if (w->unk328 >= 0x1e) {
        return;
    }
    if (w->unk328 >= 0x1c) {
        w->unk32A++;
        if (w->unk32A > 0xff) {
            w->unk32A = 0xff;
        }
    }
    if (w->unk422 & 0x100) {
        w->unkC = 1;
    }
    if (w->unk328 != 0x1a && (w->unk328 != 0x1c || w->unk32A <= 0x18)) {
        if (w->unk328 != 0x1d) {
            return;
        }
        if (w->unk32A <= 0x10) {
            return;
        }
    }
    if (w->unkC == 0) {
        return;
    }
    func_ov000_0234801c(w);
}
