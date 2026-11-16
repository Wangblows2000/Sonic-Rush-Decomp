#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 pad3[5 - 3];
    u8 unk5;
    u8 pad6[0x28 - 6];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x2E - 0x2C];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x3E - 0x36];
    s16 unk3E;
    u8 pad40[0x54 - 0x40];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x328 - 0x5E];
    s16 unk328;
    u8 pad32A[0x32C - 0x32A];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x372 - 0x334];
    s16 unk372;
    u8 pad374[0x390 - 0x374];
    u8 unk390[0x3C4 - 0x390];
    s16 unk3C4;
    u8 pad3C6[0x3DA - 0x3C6];
    s16 unk3DA;
    u8 pad3DC[0x3E0 - 0x3DC];
    s16 unk3E0;
    u8 pad3E2[0x3E4 - 0x3E2];
    u32 unk3E4;
    u8 pad3E8[0x410 - 0x3E8];
    u8 unk410;
    u8 unk411;
} UnkStruct_0234a064;

extern s16 data_0206e470[];
#define ABS(x) ((x) < 0 ? -(x) : (x))

#define IDX_0234a064(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_0234a064(a) data_0206e470[(u32)(IDX_0234a064(a) << 1)]
#define COS_0234a064(a) data_0206e470[(u32)((IDX_0234a064(a) << 1) + 1)]

void func_ov000_0234c644(UnkStruct_0234a064 *w);
BOOL func_020693d4(u8 a);
void func_0201c070(void *p, s32 a);
void func_0201bf1c(void *p);

void func_ov000_0234a064(UnkStruct_0234a064 *w, u8 ang) {
    s32 a;
    s32 b;
    s32 i;
    s32 c;
    s32 v;

    func_ov000_0234c644(w);
    w->unk3E4 = 0;
    w->unk32C &= ~0xc000;
    w->unk58 &= ~0x8000;
    w->unk58 &= ~0x10;
    w->unk58 |= 0x80;
    w->unk3E0 = 0x78;
    w->unk5C &= ~0x20;
    w->unk3C4 = 0;
    if (w->unk328 < 0x1a || w->unk328 > 0x20) {
        if (w->unk410 != 0 && (w->unk411 & 2)) {
            w->unk54 &= ~1;
            w->unk54 |= w->unk411 & 1;
            w->unk411 = 0;
        }
        w->unk410 = 0;
    }
    if (ang != 0) {
        w->unk34 += (s16)((w->unk28 * COS_0234a064((u16)(ang << 8))) >> 12);
        w->unk34 += (s16)((w->unk2A * SIN_0234a064((u16)(ang << 8))) >> 12);
        if (func_020693d4(w->unk5)) {
            w->unk34 = -w->unk34;
        }
    } else {
        a = w->unk2E < 0 ? -w->unk2E : w->unk2E;
        b = w->unk34 < 0 ? -w->unk34 : w->unk34;
        if (b < a) {
            w->unk34 = w->unk2E;
        }
        i = SIN_0234a064((u16)(w->unk1 << 8));
        c = w->unk2E < 0 ? -w->unk2E : w->unk2E;
        w->unk34 += (c * i << 4) >> 16;
    }
    v = w->unk34;
    if (v < 0) {
        v = -v;
    }
    w->unk372 = v;
    if (w->unk372 > w->unk3E) {
        w->unk372 = w->unk3E;
    }
    w->unk2E = 0;
    w->unk30 = 0;
    if (ang != 0) {
        w->unk1 = ang;
    }
    if (!(w->unk330 & 0x1000)) {
        w->unk2 = 0;
    }
    w->unk3DA = 0;
    func_0201c070(w->unk390, 0x20);
    func_0201bf1c(w->unk390);
}
