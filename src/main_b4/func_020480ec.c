#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0xc];
} UnkSub_020480ec;

typedef struct {
    void (*fn0)(void);
    u8 pad4[0xc];
    void (*fn10)(void);
    u8 pad14[0x158];
    void (*fn16c)(void);
    u8 pad170[0x90];
    void (*fn200)(void);
    u8 pad204[0x70];
    void (*fn274)(void);
    u8 pad278[0xb8];
    UnkSub_020480ec sub330;
    void (*fn340)(void);
    u8 pad344[4];
    void *res348;
    void *res34c;
    s16 unk350;
    u8 pad352[2];
    s32 unk354;
    s32 unk358;
    s32 unk35c;
    s32 unk360;
    s32 unk364;
    s32 unk368;
    s32 unk36c;
    s32 unk370;
    s32 unk374;
    s32 unk378;
    s32 unk37c;
    s32 unk380;
    s32 unk384;
    u32 unk388;
    u32 unk38c;
    u32 unk390;
    void *res394;
    void *unk398;
    u8 pad39c[0x1630 - 0x39c];
    s32 unk1630;
    u8 unk1634[4];
} UnkWork_020480ec;

typedef struct { u8 pad0[0x10]; UnkWork_020480ec *work; } UnkTask_020480ec;

typedef struct {
    u32 unk0;
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[8];
    u8 unk1c;
    u8 unk1d;
    u8 pad1e[2];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct { u8 pad0[8]; s16 unk8; } UnkBright_020480ec;
typedef struct { u8 pad0[4]; u16 unk4; u16 unk6; } UnkStruct_0208922c;
typedef struct { u8 pad0[8]; u16 unk8; u16 unka; } UnkStruct_02369ac0;
typedef struct { u32 data[0x17]; } UnkArc_020480ec;

extern UnkStruct_022c4560 data_022c4560;
extern UnkBright_020480ec data_0208926c;
extern UnkBright_020480ec data_02087d44;
extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern u32 data_ov000_0236ac50[][2];
extern u32 data_ov000_0236ac40;
extern u32 data_ov000_0236ac48;
extern char data_0207923c[];
extern char data_02079240[];
extern char data_02079258[];
extern void *gActComArchive;
extern void *gSoundHeap;

void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void NNS_SndStopSoundAll(void);
UnkTask_020480ec *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void func_02047ecc(void);
void func_02047d2c(void);
void func_02045d10(void);
void func_020473d8(void);
void func_020471ec(void);
void func_02046d20(void);
void func_02047010(void);
void func_02045df0(void);
void func_02045814(void);
void func_02046608(void);
void func_02045ea8(UnkSub_020480ec *s);
void func_0204594c(void *p);
void func_02046738(void *p);
void func_020598bc(u8 a, u8 b, u16 c, u16 d);
void func_0204e4dc(void);
void NNS_SndHandleInit(void *p);
void func_02022270(s32 i, u16 v);
void func_02022284(s32 i, s32 x, s32 y, s32 z);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForSubBGExtPltt(s32 a);
void G3X_SetClearColor(u32 a, u32 b, u16 c, u32 d, BOOL e);
BOOL func_02021310(UnkArc_020480ec *arc, const char *name, void *narc);
void *func_02021264(UnkArc_020480ec *arc, s32 idx);
BOOL func_020212e0(UnkArc_020480ec *arc);
void *File_Load(void *a, s32 b);
void func_02047b78(UnkWork_020480ec *w);
void func_020479d8(UnkWork_020480ec *w);
void func_020478f8(UnkWork_020480ec *w);
void func_0204771c(UnkWork_020480ec *w);
void func_020477f8(UnkWork_020480ec *w);
void NaviMsg_Create(void);
void func_02053b04(s32 a);
void func_02044d44(void);
void Sound_ResetHeap(void);
void func_02036854(void *p);
void NNS_SndArcLoadGroup(s32 a, void *b);
void Sound_PlaySeq(s32 a, s32 b);

#pragma thumb on
void func_020480ec(void) {
    UnkWork_020480ec *w;
    s32 a;
    s32 b;
    vu16 zero;
    UnkArc_020480ec arc;

    GXx_SetMasterBrightness_((vu16 *)0x400006c, -16);
    data_0208926c.unk8 = -16;
    GXx_SetMasterBrightness_((vu16 *)0x400106c, -16);
    data_02087d44.unk8 = -16;
    NNS_SndStopSoundAll();
    w = Task_Create(func_02047ecc, func_02047d2c, 0, 0x3efe, 3, sizeof(UnkWork_020480ec))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_020480ec));
    w->fn340 = func_02045d10;
    w->fn0 = func_020473d8;
    w->fn16c = func_020471ec;
    w->fn10 = func_02046d20;
    w->fn200 = func_02047010;
    w->sub330.fn = func_02045df0;
    func_02045ea8(&w->sub330);
    if (data_022c4560.unk10 == 2) {
        func_0204594c(&w->fn274);
        w->fn274 = func_02045814;
        func_020598bc(data_022c4560.unk1c, data_022c4560.unk1d, data_022c4560.unk20, data_022c4560.unk0);
    } else {
        func_02046738(&w->fn274);
        w->fn274 = func_02046608;
        func_0204e4dc();
    }
    NNS_SndHandleInit(w->unk1634);
    w->unk350 = 0x1555;
    w->unk354 = 0x1000;
    w->unk358 = 0x800000;
    w->unk35c = 0x1555;
    w->unk364 = 0x80000;
    w->unk368 = -0x86666;
    w->unk36c = 0;
    w->unk370 = 0x80000;
    w->unk374 = -0x60000;
    w->unk378 = 0;
    w->unk37c = 0;
    w->unk380 = 0x1000;
    w->unk384 = 0;
    func_02022270(0, 0x6f7b);
    func_02022284(0, 0, -0x4a0, -0xf40);
    func_02022270(1, 0x2108);
    func_02022284(1, 0xec7, -0x600, 0);
    func_02022270(2, 0x2108);
    func_02022284(2, 0, 0x620, 0xec7);
    if (data_022c4560.unk1c >= 9 || data_022c4560.unk1d >= 2) {
        GX_SetGraphicsMode(1, 5, 1);
        Vram_SetBankForBGExtPltt(0);
        Vram_SetBankForSubBGExtPltt(0);
        G3X_SetClearColor(0, 0, 0x7fff, 0, 0);
        *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 1;
        *(vu16 *)0x400000a = (*(vu16 *)0x400000a & ~3) | 3;
        *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 0;
        *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 2;
    } else {
        *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0xe808;
    }
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 0x2888;
    gDisplayMain.unk4 = 0;
    gDisplayMain.unk6 = 0;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x4000;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & 0x43) | 0x4400;
    w->unk388 = data_ov000_0236ac50[data_022c4560.unk0][0];
    w->unk38c = data_ov000_0236ac40;
    w->unk390 = data_ov000_0236ac48;
    func_02021310(&arc, data_0207923c, gActComArchive);
    w->res348 = func_02021264(&arc, 0x1a);
    w->res34c = func_02021264(&arc, 0x1b);
    w->res394 = func_02021264(&arc, 0x19);
    func_020212e0(&arc);
    if (data_022c4560.unk10 == 2)
        w->unk398 = File_Load(data_02079240, 0);
    func_02047b78(w);
    if (data_022c4560.unk10 == 2)
        func_020479d8(w);
    func_020478f8(w);
    if (data_022c4560.unk1c >= 9 || data_022c4560.unk1d >= 2)
        func_0204771c(w);
    else
        func_020477f8(w);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1f00;
    if (data_022c4560.unk1c >= 9 || data_022c4560.unk1d >= 2) {
        NaviMsg_Create();
        func_02053b04(1);
        data_ov000_02369ac0.unk8 = 0;
        data_ov000_02369ac0.unka = 8;
        func_02044d44();
    }
    Sound_ResetHeap();
    func_02036854(data_02079258);
    if (data_022c4560.unk10 == 2) {
        if (data_022c4560.unk1c >= 9 || data_022c4560.unk1d >= 2) {
            a = 6;
            w->unk1630 = 5;
            b = 0xd;
        } else {
            a = 5;
            w->unk1630 = 5;
            b = 0xb;
        }
    } else if (data_022c4560.unk1c == 7) {
        a = 9;
        w->unk1630 = 7;
        b = 0x11;
    } else if (data_022c4560.unk1c >= 9 || data_022c4560.unk1d >= 2) {
        a = 8;
        w->unk1630 = 6;
        b = 0x10;
    } else {
        a = 7;
        w->unk1630 = 6;
        b = 0xf;
    }
    NNS_SndArcLoadGroup(a, gSoundHeap);
    Sound_PlaySeq(0, b);
}
#pragma thumb reset
