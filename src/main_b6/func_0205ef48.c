#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_0205ef48;

typedef struct {
    u16 h0;
    u16 pad2;
    u32 w4;
    u32 w8;
    u32 wc;
    u32 w10;
} UnkCam_0205ef48;

typedef void (*Fn_0205ef48)(void);

typedef struct {
    Fn_0205ef48 fn0;
    UnkObj_0205ef48 *obj;
    u8 pad8[0x18];
    Fn_0205ef48 fn20;
    u8 pad24[0x54];
    Fn_0205ef48 fn78;
    u8 pad7c[0xc];
    Fn_0205ef48 fn88;
    u8 pad8c[0x54];
    Fn_0205ef48 fne0;
    u8 pade4[0x9c];
    Fn_0205ef48 fn180;
    u8 pad184[0x64];
    u32 unk1e8;
    u32 pad1ec;
    Fn_0205ef48 fn1f0;
    u8 pad1f4[0x64];
    u32 unk258;
    u16 unk25c;
    u16 pad25e;
    Fn_0205ef48 fn260;
    u8 pad264[0xc0];
    Fn_0205ef48 fn324;
    u8 pad328[0xc0];
    Fn_0205ef48 fn3e8;
    u8 pad3ec[0x18];
    Fn_0205ef48 fn404;
    u8 pad408[0x18];
    Fn_0205ef48 fn420;
    u8 pad424[0x50];
    Fn_0205ef48 fn474;
    Fn_0205ef48 fn478;
    u32 unk47c;
    s32 objX;
    s32 objY;
    u32 flags;
    u32 pad48c;
    void *res[4];
    u16 camH;
    u16 pad4a2;
    u32 cam4;
    u32 cam8;
    u32 camC;
    u32 cam10;
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
} UnkWork_0205ef48;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0205ef48 *work;
} UnkTask_0205ef48;

extern UnkWork_0205ef48 *data_022c4d98;
extern void *data_ov001_0231f528;
extern char data_0207c610[];
extern u8 data_02072cb8;
extern u16 data_02076b5c[];

void *func_020372d0(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
u8 *func_ov001_022ffce8(void);
UnkCam_0205ef48 *func_ov001_022f71f4(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_0205e378(UnkWork_0205ef48 *w);
void func_0205df04(UnkWork_0205ef48 *w);
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

void func_0205ef48(void) {
    UnkWork_0205ef48 *w;
    UnkCam_0205ef48 *cam;
    u32 v;
    u32 f[0x18];

    w = ((UnkTask_0205ef48 *)func_020372d0(func_0205ec5c, func_0205eb68, 0, 0x3efe, 3, 0x8d0))->work;
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
    w->obj = (UnkObj_0205ef48 *)(func_ov001_022ffce8() + 0x64);
    w->objX = w->obj->x;
    w->objY = w->obj->y;
    w->flags |= 0x80025;
    w->unk4d8 = 0x1f;
    w->unk1e8 = 0;
    w->unk258 = 0x10000;
    w->unk25c = 0xf;
    w->unk47c = 0;
    cam = func_ov001_022f71f4();
    w->camH = cam->h0;
    w->cam4 = cam->w4;
    w->cam8 = cam->w8;
    w->camC = cam->wc;
    w->cam10 = cam->w10;
    w->v4b4 = 0x9e000;
    w->v4b8 = -0x5b000;
    w->v4bc = 0x1e000;
    w->v4c0 = 0x80000;
    w->v4c4 = -0x5b000;
    w->v4c8 = 0;
    w->v4cc = 0;
    w->v4d0 = 0x1000;
    w->v4d4 = 0;
    REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | 0x1500;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | 0x1c00;
    func_02021310(f, data_0207c610, data_ov001_0231f528);
    w->res[0] = func_02021264(f, 0);
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        v = data_02072cb8;
        break;
    default:
        v = 1;
        break;
    }
    w->res[1] = func_02021264(f, data_02076b5c[v]);
    w->res[2] = func_02021264(f, 4);
    w->res[3] = func_02021264(f, 3);
    func_020212e0(f);
    func_0205e378(w);
    func_0205df04(w);
    func_02022270(0, 0);
    func_02022270(1, 0);
    func_02022270(2, 0);
    {
        u16 edge[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        func_02004084(edge);
    }
}
