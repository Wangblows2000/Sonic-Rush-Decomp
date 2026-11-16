#include <nitro/types.h>

typedef struct UnkStruct_02353568 UnkStruct_02353568;

struct UnkStruct_02353568 {
    u8 pad0;
    u8 unk1;
    u8 pad2;
    u8 unk3;
    u8 pad4[0x2E - 4];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x32A - 0x5E];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x334 - 0x330];
    void (*unk334)(UnkStruct_02353568 *w);
    u8 pad338[0x388 - 0x338];
    u8 unk388[0x3F0 - 0x388];
    s32 unk3F0;
};

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02353568_4560;

extern s16 data_0206e470[];
extern UnkStruct_02353568_4560 data_022c4560;

void func_0201c070(void *p, s32 a);
void func_0201bf1c(void *p);
void func_ov000_0234c644(UnkStruct_02353568 *w);
void func_ov000_0234a064(UnkStruct_02353568 *w, u8 ang);
void func_ov000_02328e6c(UnkStruct_02353568 *w, s32 a, s32 b);
void func_ov000_02347680(UnkStruct_02353568 *w);
void StageObj_Move(UnkStruct_02353568 *w);

#define IDX_02353568(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02353568(a) data_0206e470[(u32)(IDX_02353568(a) << 1)]
#define COS_02353568(a) data_0206e470[(u32)((IDX_02353568(a) << 1) + 1)]

void func_ov000_02353568(UnkStruct_02353568 *w) {
    s16 spd = 0;

    if (w->unk32A != 0) {
        w->unk32A--;
    } else if (w->unk58 & 0xd) {
        func_0201c070(w->unk388, 0x10);
        func_0201bf1c(w->unk388);
        func_ov000_0234c644(w);
        w->unk1 = 0;
        w->unk58 &= ~0x2000;
        w->unk58 |= 0x80;
        w->unk58 |= 0x20000;
        w->unk32C &= ~0x100000;
        w->unk2E = w->unk34;
        func_ov000_0234a064(w, 0);
        w->unk334(w);
        return;
    }
    if (!(data_022c4560.unk20 & 7)) {
        func_ov000_02328e6c(w, 0, 0xc);
    }
    if (w->unk5C & 1) {
        w->unk1 = 0xf6;
    } else {
        w->unk1 = 0xa;
    }
    func_ov000_02347680(w);
    if (w->unk3F0 != 0) {
        spd = w->unk34;
        if (w->unk5C & 1) {
            w->unk3F0 += spd;
        } else {
            w->unk3F0 -= spd;
        }
        w->unk3 = w->unk3F0 >> 8;
        w->unk34 = (w->unk34 * COS_02353568((u16)(w->unk3 << 8))) >> 12;
        if (w->unk5C & 1) {
            w->unk1 += (u8)(((0x100 - w->unk1) * SIN_02353568((u16)(w->unk3 << 8)) * 0x290) >> 20);
        } else {
            w->unk1 -= (u8)((w->unk1 * SIN_02353568((u16)(w->unk3 << 8))) >> 12);
        }
        if (w->unk3F0 < 0) {
            if (w->unk3 <= 0xc0) {
                w->unk1 = -w->unk1;
            }
            if (w->unk3 <= 0x80) {
                w->unk3F0 = 0;
                w->unk3 = 0;
                w->unk5C ^= 1;
                spd = -spd;
            }
        } else {
            if (w->unk3 >= 0x40) {
                w->unk1 = -w->unk1;
            }
            if (w->unk3 >= 0x80) {
                w->unk3F0 = 0;
                w->unk3 = 0;
                w->unk5C ^= 1;
                spd = -spd;
            }
        }
    }
    w->unk58 &= ~0x2000;
    StageObj_Move(w);
    w->unk58 |= 0x2000;
    w->unk1 = 0;
    if (spd != 0) {
        w->unk34 = spd;
    }
}
