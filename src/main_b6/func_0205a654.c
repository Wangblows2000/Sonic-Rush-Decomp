#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 6;
            u16 b6 : 2;
            u16 b8 : 1;
            u16 b9 : 1;
            u16 b10 : 1;
            u16 b11 : 1;
            u16 b12 : 4;
        } f;
    } u;
} UnkDisp_0205a654;

typedef struct {
    u8 pad0[0x5c];
    u16 flags5c;
    u8 pad5e[0x12];
    u32 unk70;
} UnkObj_0205a654;

typedef struct {
    u8 pad0[0x14];
    UnkObj_0205a654 *obj2;
    u32 unk18;
    u8 pad1c[0x458];
    void (*fn474)(void);
    u8 pad478[0x454];
    u16 unk8cc;
} UnkWork_0205a654;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[8];
    u8 zone;
    u8 act;
} UnkState_0205a654;

extern UnkState_0205a654 data_022c4560;
extern UnkDisp_0205a654 *gDisplayTable[];

void func_ov000_023375e8(void);
void func_0204cdc0(void);
void func_0205a630(void);

void func_0205a654(UnkWork_0205a654 *w) {
    if (data_022c4560.zone != 8 || data_022c4560.act != 2) {
        func_ov000_023375e8();
        func_0204cdc0();
    } else {
        UnkDisp_0205a654 *d = gDisplayTable[0];
        d->u.raw = 0;
        d->u.f.b6 = 1;
        d->u.f.b8 = 1;
        d->u.f.b9 = 1;
        d->u.f.b10 = 1;
        d->u.f.b11 = 1;
    }
    if (data_022c4560.unk10 == 1) {
        w->obj2->unk70 = w->unk18;
        w->obj2->flags5c |= 0x40;
    }
    w->unk8cc = 0;
    w->fn474 = func_0205a630;
}
