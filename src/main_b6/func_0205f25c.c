#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 flags8;
} UnkSub3_0205f25c;

typedef struct {
    u8 pad0[8];
    UnkSub3_0205f25c *p8;
} UnkSub2_0205f25c;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x44];
    u16 flags5c;
    u8 pad5e[0x12];
    void (*fn70)(void);
    u8 pad74[0x1a4];
    UnkSub2_0205f25c *p218;
    u8 pad21c[0x110];
    u32 flags32c;
} UnkObj_0205f25c;

typedef struct {
    u32 w[5];
} UnkCam_0205f25c;

typedef void (*Fn_0205f25c)(void);

typedef struct {
    Fn_0205f25c fn0;
    UnkObj_0205f25c *obj;
    void (*fn8)(void);
    u8 padc[8];
    UnkObj_0205f25c *obj2;
    void (*fn18)(void);
    u8 pad1c[4];
    Fn_0205f25c fn20;
    u8 pad24[0x54];
    Fn_0205f25c fn78;
    u8 pad7c[0xc];
    Fn_0205f25c fn88;
    u8 pad8c[0x54];
    Fn_0205f25c fne0;
    u8 pade4[0x9c];
    Fn_0205f25c fn180;
    u8 pad184[0x64];
    u32 unk1e8;
    u32 pad1ec;
    Fn_0205f25c fn1f0;
    u8 pad1f4[0x64];
    u32 unk258;
    u16 unk25c;
    u16 pad25e;
    Fn_0205f25c fn260;
    u8 pad264[0xc0];
    Fn_0205f25c fn324;
    u8 pad328[0xc0];
    Fn_0205f25c fn3e8;
    u8 pad3ec[0x18];
    Fn_0205f25c fn404;
    u8 pad408[0x18];
    Fn_0205f25c fn420;
    u8 pad424[0x50];
    Fn_0205f25c fn474;
    Fn_0205f25c fn478;
    u32 unk47c;
    s32 objX;
    s32 objY;
    u32 flags;
    u32 pad48c;
    void *res[4];
    UnkCam_0205f25c cam;
    s32 v4b4;
    s32 v4b8;
    s32 v4bc;
    s32 v4c0;
    s32 v4c4;
    s32 v4c8;
    s32 v4cc;
    s32 v4d0;
    s32 v4d4;
    u16 unk4d8;
} UnkWork_0205f25c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0205f25c *work;
} UnkTask_0205f25c;

typedef struct {
    s32 unk0;
    u8 pad4[0xc];
    s32 unk10;
    u32 pad14;
    u32 flags;
    u8 zone;
    u8 act;
} UnkState_0205f25c;

typedef struct {
    u8 pad0[0x15];
    u8 b15;
} UnkState2_0205f25c;

typedef struct {
    u32 unk0;
    u8 pad4[0x30];
    u32 unk34;
    u8 pad38[0x2c];
    u32 unk64;
    u8 pad68[0x30];
    u32 unk98;
    u8 pad9c[0x2c];
    u32 unkc8;
} UnkGfx_0205f25c;

typedef struct {
    u32 flags;
} UnkG_0205f25c;

extern UnkWork_0205f25c *data_022c4d98;
extern void *gActComArchive;
extern char data_0207c614[];
extern u8 gGameLanguage;
extern u16 data_02076b74[];
extern UnkG_0205f25c data_ov000_02369ac0;
extern UnkObj_0205f25c *data_ov000_0236ac60[];
extern UnkState_0205f25c data_022c4560;
extern UnkState2_0205f25c data_022c4590;
extern UnkCam_0205f25c data_ov000_0236a448;
extern UnkGfx_0205f25c data_ov000_0236aad8;

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0205e09c(UnkWork_0205f25c *w);
void func_ov000_023375c8(void);
void func_0205d604(void);
void func_0205b478(void);
void func_0205b3b8(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_0205e378(UnkWork_0205f25c *w);
void func_0205df04(UnkWork_0205f25c *w);
void func_02022270(u32 index, u32 value);
void func_02004084(const u16 *table);
void func_0205ec5c(void);
void func_0205eb68(void);
void func_0205b39c(void);
void func_0205b560(void);
void func_0205c6d0(void);
void func_0205c5b8(void);
void func_0205c498(void);
void func_0205c328(void);
void func_0205bf84(void);
void func_0205bba4(void);
void func_0205b890(void);
void func_0205b6f0(void);
void func_0205b574(void);
void func_0205cfd0(void);
void func_0205c708(void);

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void func_0205f25c(void) {
    UnkWork_0205f25c *w;
    u32 v;
    BOOL b;
    BOOL b2;
    u32 f[0x18];

    data_ov000_02369ac0.flags &= ~1;
    w = ((UnkTask_0205f25c *)Task_Create(func_0205ec5c, func_0205eb68, 0, 0x3efe, 3, 0x8d0))->work;
    data_022c4d98 = w;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, 0x8d0);
    }
    w->fn474 = func_0205b39c;
    w->fn478 = func_0205b560;
    w->fn78 = func_0205c6d0;
    w->fn88 = func_0205c5b8;
    w->fne0 = func_0205c498;
    w->fn180 = func_0205c328;
    w->fn1f0 = func_0205c328;
    w->fn260 = func_0205bf84;
    w->fn324 = func_0205bba4;
    w->fn3e8 = func_0205b890;
    w->fn404 = func_0205b6f0;
    w->fn420 = func_0205b574;
    w->fn0 = func_0205cfd0;
    w->fn20 = func_0205c708;
    w->obj = data_ov000_0236ac60[0];
    w->objX = w->obj->x;
    w->objY = w->obj->y;
    w->fn8 = w->obj->fn70;
    w->obj->flags5c &= ~0x40;
    if (data_022c4560.unk10 == 1) {
        w->obj2 = data_ov000_0236ac60[1];
        w->fn18 = w->obj2->fn70;
        w->obj2->flags5c &= ~0x40;
        w->obj2->fn70 = func_0205d604;
    }
    w->flags |= 0x80025;
    w->unk4d8 = 0x1f;
    w->unk1e8 = 0;
    w->unk258 = 0x10000;
    w->unk25c = 0xf;
    w->unk47c = data_022c4560.unk0 == 1;
    w->obj->flags32c |= 0x200000;
    w->obj->flags32c |= 0x400000;

    b = FALSE;
    if (data_022c4560.zone < 9 && data_022c4560.act < 2)
        b = TRUE;
    if (b) {
        u8 zone = data_022c4560.zone;
        if (zone == 0 || zone == 1 || (zone == 3 && data_022c4560.act == 0) || (zone == 4 && data_022c4560.act == 0)
            || (zone == 5 && data_022c4560.act == 1) || zone == 6) {
            if ((data_022c4560.flags & 2) == 0) {
                w->obj->p218->p8->flags8 |= 1;
                w->fn478 = func_0205b478;
            }
        } else if ((data_022c4560.flags & 2) == 0 || data_022c4590.b15 == 0) {
            w->obj->p218->p8->flags8 |= 1;
            w->flags |= 0x800000;
            w->fn478 = func_0205b3b8;
        }
    }
    w->cam = data_ov000_0236a448;
    w->v4b4 = 0x9e000;
    w->v4b8 = -0x5b000;
    w->v4bc = 0x1e000;
    w->v4c0 = 0x80000;
    w->v4c4 = -0x5b000;
    w->v4c8 = 0;
    w->v4cc = 0;
    w->v4d0 = 0x1000;
    w->v4d4 = 0;
    data_ov000_0236aad8.unk0 &= ~3;
    data_ov000_0236aad8.unk64 &= ~3;
    if (data_022c4560.zone == 1)
        data_ov000_0236aad8.unkc8 |= 0x2000000;
    data_ov000_0236aad8.unkc8 |= 0x800;
    data_022c4560.flags &= ~1;
    data_ov000_0236aad8.unk34 |= 1;
    data_ov000_0236aad8.unk98 |= 1;
    if (data_022c4560.zone >= 9 || data_022c4560.act >= 2) {
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | 0x1500;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | 0x1c00;
    } else {
        *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0xa000;
        *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0xa000;
        *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & 0x43) | 0x4400;
    }
    func_02021310(f, data_0207c614, gActComArchive);
    w->res[0] = func_02021264(f, 2);
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        v = gGameLanguage;
        break;
    default:
        v = 1;
        break;
    }
    w->res[1] = func_02021264(f, data_02076b74[v]);
    b2 = FALSE;
    if (data_022c4560.zone < 9 && data_022c4560.act < 2)
        b2 = TRUE;
    if (!b2) {
        w->res[2] = func_02021264(f, data_022c4560.zone + 0x10);
        w->res[3] = func_02021264(f, 0xf);
    } else {
        w->res[2] = func_02021264(f, data_022c4560.zone + 6);
        w->res[3] = func_02021264(f, 5);
    }
    func_020212e0(f);
    func_0205e378(w);
    if (data_022c4560.zone >= 9 || data_022c4560.act >= 2)
        func_0205df04(w);
    else
        func_0205e09c(w);
    func_02022270(0, 0);
    func_02022270(1, 0);
    func_02022270(2, 0);
    {
        u16 edge[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        func_02004084(edge);
    }
    func_ov000_023375c8();
}
