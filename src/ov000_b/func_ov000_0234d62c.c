#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[4];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
    s32 unk20;
} UnkStruct_022c4560;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u32 unk24;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s32 unk30;
    s32 unk34;
    s16 unk38;
    s16 unk3A;
    s32 unk3C;
    u8 pad40[4];
    u8 unk44;
    u8 unk45;
    u8 pad46[0x50 - 0x46];
    u8 *unk50;
    u8 *unk54;
} UnkStruct_022c4590;

typedef struct {
    u8 pad0[0xE];
    u8 unkE;
    u8 padF[0x66 - 0xF];
    u8 unk66;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x17];
    u8 unk17;
    u8 unk18;
} UnkStruct_0234d62c_Info;

typedef struct {
    void *unk0;
    u32 unk4;
} UnkStruct_0236ac50;

typedef struct UnkStruct_0234d62c {
    u8 unk0;
    u8 pad1[0x10 - 1];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x54 - 0x1C];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x6C - 0x5E];
    void *unk6C;
    void *unk70;
    void *unk74;
    void *unk78;
    void *unk7C;
    u8 pad80[0x94 - 0x80];
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 pad124[0x134 - 0x124];
    u8 unk134[0xA];
    u8 unk13E;
    u8 unk13F;
    u8 unk140;
    u8 pad141[0x14C - 0x141];
    s32 unk14C;
    s32 unk150;
    s32 unk154;
    u8 pad158[0x18C - 0x158];
    s32 unk18C;
    u8 pad190[0x1C8 - 0x190];
    UnkStruct_0234d62c_Info *unk1C8;
    u8 pad1CC[0x1F8 - 0x1CC];
    u8 *unk1F8;
    u8 *unk1FC;
    u8 pad200[0x2C4 - 0x200];
    u32 unk2C4;
    u8 pad2C8[0x324 - 0x2C8];
    u8 unk324;
    u8 unk325;
    u8 unk326;
    u8 unk327;
    u8 pad328[4];
    u32 unk32C;
    u32 unk330;
    void (*unk334)(struct UnkStruct_0234d62c *w);
    void *unk338;
    void *unk33C;
    void *unk340;
    void *unk344;
    void *unk348;
    u8 pad34C[4];
    s16 unk350;
    u8 pad352[0x376 - 0x352];
    s16 unk376;
    u8 pad378[0x3AE - 0x378];
    s16 unk3AE;
    s16 unk3B0;
    s16 unk3B2;
    u8 pad3B4[0x3CA - 0x3B4];
    s16 unk3CA;
    u8 pad3CC[0x3E4 - 0x3CC];
    s32 unk3E4;
    u8 pad3E8[0x414 - 0x3E8];
    u8 unk414[0x42C - 0x414];
    s16 unk42C;
    s16 unk42E;
    s16 unk430;
    s16 unk432;
    s16 unk434;
    s16 unk436;
    u8 pad438[0x4BC - 0x438];
} UnkStruct_0234d62c;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_022c4680 data_022c4680;
extern void *data_0208fe7c;
extern void *data_0208fe80;
extern u8 data_ov000_0236ac20;
extern s32 data_ov000_0236ac24;
extern void *data_ov000_0236ac28;
extern s32 data_ov000_0236ac2c;
extern s32 data_ov000_0236ac30;
extern UnkStruct_0236ac50 data_ov000_0236ac50[];
extern UnkStruct_0234d62c *data_ov000_0236ac60[];
extern u8 data_ov000_0236ac38[], data_ov000_0236ac40[], data_ov000_0236ac48[];
extern u8 data_ov000_02368d7c[], data_ov000_02368d88[], data_ov000_02368d94[], data_ov000_02368da0[];
extern u8 data_ov000_02368dac[];

void func_ov000_0234d384(void);
void func_ov000_0234419c(void);
void func_ov000_023439b8(void);
void func_ov000_02343940(void);
void func_ov000_02343d80(void);
void func_ov000_0234c3b4(UnkStruct_0234d62c *w);
void func_ov000_0234b758(void);
void func_ov000_0234bdac(void);
void func_ov000_0234ac58(void);
void func_ov000_02348a54(void);
void func_ov000_02348ca8(void);
void func_ov000_0234360c(void);
void func_ov000_0234cb70(UnkStruct_0234d62c *w);
void func_ov000_02331058(UnkStruct_0234d62c *w);
void func_ov000_02330aec(UnkStruct_0234d62c *w);
void func_ov000_0232fde0(UnkStruct_0234d62c *w);
void func_ov000_0233e5c8(s32 a, s32 b);
BOOL func_ov000_02345758(s32 x, s32 y);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_02069c74(void *w, void *sub, void *tbl, s32 a, void *b, void *c);
void func_02069b34(void *w, void *sub, void *tbl, void *a, void *b);
void func_0206cf8c(void *a, void *b);

#define ALLOC4_0234D62C(dst)                                  \
    do {                                                      \
        u8 *p_ = func_02034a40(data_0208fe7c, 0x3140, 4);     \
        if ((u8 *)func_02006abc(0) != p_) {                   \
            vu32 fill_ = 0xcccccccc;                          \
            MIi_CpuClear32(fill_, p_, 0x3140);                \
            *(u32 *)p_ = 0x3140;                              \
            *(u32 *)(p_ + 0x3140 - 4) = 0x02467531;           \
            DC_StoreRange(p_, 0x3140);                        \
            p_ += 4;                                          \
        }                                                     \
        dst = p_;                                             \
    } while (0)

#define ALLOC32_0234D62C(dst, n)                                \
    do {                                                        \
        u32 size_ = ((n) + 0x43) & ~0x1f;                       \
        u8 *p_ = func_02034a40(data_0208fe80, size_, 0x20);     \
        if ((u8 *)func_02006abc(0) != p_) {                     \
            vu32 fill_ = 0xcccccccc;                            \
            MIi_CpuClear32(fill_, p_, size_);                   \
            *(u32 *)p_ = size_;                                 \
            *(u32 *)(p_ + size_ - 4) = 0x02467531;              \
            DC_StoreRange(p_, size_);                           \
            p_ += 0x20;                                         \
        }                                                       \
        dst = p_;                                               \
    } while (0)

static inline void Init_0234d62c(UnkStruct_022c4590 *p, s32 type) {
    if (!(p->unk24 & 2) && !(data_022c4560.unk18 & 0x40)) {
        if (!(p->unk24 & 1)) {
            p->unk24 &= ~4;
            if (type != 0) {
                if (type == 1)
                    p->unk14 = data_022c4680.unk66;
            } else {
                p->unk14 = data_022c4680.unkE;
            }
        }
        data_022c4590.unk24 |= 1;
        data_022c4590.unk0 = 0;
        data_022c4590.unk4 = 0;
        data_022c4590.unk8 = 0;
        data_022c4590.unkC = 0;
        data_022c4590.unk10 = 0;
        data_022c4590.unk38 = 0;
        data_022c4590.unk18 = 0;
        data_022c4590.unk24 |= 2;
        data_022c4590.unk3A = 0;
        data_022c4590.unk44 = 0;
        data_022c4590.unk45 = 0;
        data_022c4590.unk15 = 0;
        if (data_022c4560.unk10 == 2) {
            if (data_022c4590.unk54 == NULL)
                ALLOC4_0234D62C(data_022c4590.unk54);
            if (data_022c4590.unk50 == NULL)
                ALLOC4_0234D62C(data_022c4590.unk50);
        }
    }
}

static inline void InitBufs_0234d62c(UnkStruct_0234d62c *w) {
    func_02069b34(w, w->unk134, data_ov000_02368d94, data_ov000_0236ac40, data_ov000_0236ac28);
    func_02069b34(w, w->unk134, data_ov000_02368da0, data_ov000_0236ac48, data_ov000_0236ac28);
    ALLOC32_0234D62C(w->unk1F8, w->unk1C8->unk17 * 0x58);
    ALLOC32_0234D62C(w->unk1FC, w->unk1C8->unk18 * 0x38);
    func_02069f1c(w, data_ov000_02368dac, 1, data_ov000_0236ac38, data_ov000_0236ac28);
    w->spr0.unk46 = 0xa;
    w->spr1.unk46 = 0xa;
    w->spr0.unk2C |= 0x80018;
    w->spr1.unk2C |= 0x80018;
}

UnkStruct_0234d62c *func_ov000_0234d62c(s32 type, u8 id) {
    UnkStruct_Task *task;
    UnkStruct_0234d62c *w;
    UnkStruct_022c4590 *g = &data_022c4590;
    vu16 zero;
    BOOL flag;

    task = func_020372d0(func_0206c3d0, func_ov000_0234d384, 0, 0x100, 1, sizeof(UnkStruct_0234d62c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0234d62c));
    if (data_ov000_0236ac20 == 0)
        Init_0234d62c(&data_022c4590, type);
    data_ov000_0236ac60[data_ov000_0236ac20] = w;
    w->unk325 = data_ov000_0236ac20;
    w->unk327 = data_ov000_0236ac20;
    data_ov000_0236ac20++;
    w->unk326 = id;
    w->unk324 = type;
    w->unk0 = 1;
    w->unk54 |= 0x10;
    w->unk54 |= 1;
    w->unk6C = func_ov000_0234419c;
    w->unk74 = func_ov000_023439b8;
    w->unk78 = func_ov000_02343940;
    w->unk7C = func_ov000_02343d80;
    w->unk5C |= 0x800;
    w->unk42C = 0x10;
    w->unk42E = 0x20;
    w->unk430 = 0x40;
    w->unk432 = 0x80;
    w->unk434 = 1;
    w->unk436 = 2;
    if (w->unk324 == 1)
        func_02069c74(w, w->unk134, data_ov000_02368d7c, 0, &data_ov000_0236ac50[w->unk324], data_ov000_0236ac28);
    else
        func_02069c74(w, w->unk134, data_ov000_02368d88, 0, &data_ov000_0236ac50[w->unk324], data_ov000_0236ac28);
    w->unk5C |= 0x200;
    w->unk14C = 0x34cc;
    w->unk150 = 0x34cc;
    w->unk154 = 0x34cc;
    w->unk18C = -0x10000;
    w->unk13E = 4;
    w->unk13F = 1;
    w->unk140 = 0xf;
    InitBufs_0234d62c(w);
    func_0206cf8c(data_ov000_0236ac50[w->unk324].unk0, w->unk414);
    w->unk334 = func_ov000_0234c3b4;
    w->unk338 = func_ov000_0234b758;
    w->unk33C = func_ov000_0234bdac;
    w->unk340 = func_ov000_0234ac58;
    if (type == 1)
        w->unk344 = func_ov000_02348a54;
    else
        w->unk344 = func_ov000_02348ca8;
    w->unk348 = NULL;
    if (!(data_022c4560.unk18 & 0x20))
        data_022c4560.unk20 = data_022c4590.unk18;
    w->unk3CA = 0;
    w->unk3E4 = 0;
    func_ov000_0234cb70(w);
    if ((data_022c4560.unk18 & 0x40) && w->unk325 == 0) {
        w->unk3AE = data_022c4590.unk28;
        w->unk3B0 = data_022c4590.unk2A;
        data_022c4560.unk20 = data_022c4590.unk3C;
        switch (data_022c4590.unk2C) {
        case 1:
            w->unk32C |= 0x10000000;
            func_ov000_02331058(w);
            break;
        case 2:
            w->unk32C |= 0x20000000;
            func_ov000_02330aec(w);
            break;
        }
        w->unk350 = data_022c4590.unk2E;
        w->unk10 = data_022c4590.unk30 << 8;
        w->unk14 = data_022c4590.unk34 << 8;
        w->unk330 |= 0x200000;
        w->unk3B2 = w->unk376;
        w->unk2C4 |= 5;
        if (data_022c4590.unk3A != 0) {
            func_ov000_0233e5c8(3, 8);
            w->unk330 |= 0x8000;
        }
    } else {
        if (g->unk1C == 0 && g->unk20 == 0) {
            g->unk1C = 0x28;
            g->unk20 = 0x78;
        }
        w->unk10 = g->unk1C << 8;
        w->unk14 = g->unk20 << 8;
        data_ov000_0236ac2c = w->unk10 >> 8;
        data_ov000_0236ac30 = w->unk14 >> 8;
        data_ov000_0236ac24 = w->unk18 >> 8;
        if (data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2)
            w->unk350 = 0x640;
        if (func_ov000_02345758(w->unk10, w->unk14))
            w->unk5C ^= 1;
    }
    w->unk334(w);
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        data_ov000_0236ac60[0]->unk5C &= ~0x200;
        w->unk5C &= ~0x800;
        w->unk5C &= ~0xa00;
    } else {
        data_ov000_0236ac60[0]->unk70 = func_ov000_0234360c;
        w->unk5C |= 0x40;
    }
    if ((data_022c4560.unk18 & 0x20) || w->unk325 != 0)
        func_ov000_0232fde0(w);
    if (data_022c4560.unk18 & 0x20)
        w->unk3AE = 1;
    flag = FALSE;
    if (data_022c4560.unk1C >= 7 && data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2)
        flag = TRUE;
    if (flag)
        data_022c4560.unk20 = 1;
    return w;
}
