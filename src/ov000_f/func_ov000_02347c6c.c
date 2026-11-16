#include <nitro/types.h>

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkStruct_02347c6c_Pal;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x54 - 6];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x325 - 0x5C];
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x3B4 - 0x334];
    s16 unk3B4;
    s16 unk3B6;
    u8 pad3B8[0x3E8 - 0x3B8];
    void *unk3E8;
    u8 pad3EC[0x414 - 0x3EC];
    UnkStruct_02347c6c_Pal pal;
    u16 unk420;
    u16 unk422;
    u16 unk424;
} UnkStruct_02347c6c;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x1C - 0x14];
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[0x20 - 0x1E];
    u32 unk20;
} UnkStruct_02347c6c_4560;

typedef struct {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_02347c6c_4590;

extern UnkStruct_02347c6c_4560 data_022c4560;
extern UnkStruct_02347c6c_4590 data_022c4590;
extern UnkStruct_02347c6c *data_ov000_0236ac60[];

void func_ov000_0234c9e8(UnkStruct_02347c6c *w);
BOOL BrightnessFade_IsDone(void);
void BrightnessFade_Create(s32 a, s32 b);
void func_0206cdf4(UnkStruct_02347c6c_Pal *p, s32 a, s32 b, s32 c);

void func_ov000_02347c6c(UnkStruct_02347c6c *w, void *o) {
    BOOL ok;
    UnkStruct_02347c6c *p;

    if (w->unk32C & 0x200000) {
        return;
    }
    func_ov000_0234c9e8(w);
    w->unk3E8 = o;
    w->unk330 |= 0x30;
    ok = FALSE;
    w->unk32C |= 0x200000;
    w->unk32C |= 0x400000;
    w->unk32C |= 0x1000000;
    if (data_022c4560.unk1C >= 7 && data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2) {
        ok = TRUE;
    }
    if (!ok) {
        w->unk54 |= 2;
    } else {
        w->unk3E8 = NULL;
        w->unk330 &= ~0x30;
    }
    if ((data_022c4560.unk1C == 2 && data_022c4560.unk1D == 1) ||
        (data_022c4560.unk10 == 1 && (data_022c4590.unk40 >= 2 || data_022c4560.unk20 >= 0x8c9f))) {
        w->unk420 = 0;
        w->unk422 = 0;
        w->unk424 = 0;
        if (data_022c4560.unk10 == 1 && !BrightnessFade_IsDone() && data_022c4590.unk40 == 3) {
            data_022c4590.unk40 = 2;
            BrightnessFade_Create(4, 0);
        }
    } else {
        w->unk58 &= ~0x80000;
        w->unk422 = 0;
        w->unk424 = 0;
        w->unk420 &= 0xc03;
        if (w->unk5 != 0) {
            w->unk420 |= 0x20;
        } else {
            w->unk420 |= 0x10;
        }
    }
    w->unk3B6 = 0;
    w->unk3B4 = 0;
    func_0206cdf4(&w->pal, 0, 0, 0);
    if (data_022c4560.unk10 != 1) {
        return;
    }
    if (w->unk325 == 0) {
        return;
    }
    p = data_ov000_0236ac60[0];
    p->unk32C |= 0x400000;
    p->unk32C |= 0x1000000;
    p->unk3B6 = 0;
    p->unk3B4 = 0;
    func_0206cdf4(&p->pal, 0, 0, 0);
}
