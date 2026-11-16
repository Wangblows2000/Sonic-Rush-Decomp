#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x2A];
    u16 unk2A;
    u32 unk2C;
} UnkStruct_0235c238_ResHdr;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_0235c238_Res;

typedef struct {
    u8 pad0[4];
    void *unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    UnkStruct_Spr spr0;
    UnkStruct_Spr spr1;
    u8 unkA4[0xA];
    u8 unkAE;
    u8 unkAF;
    u8 padB0[0xBC - 0xB0];
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    u8 padC8[0x160 - 0xC8];
    u32 unk160;
    u8 pad164[0x198 - 0x164];
    s32 unk198;
    s32 unk19C;
    u8 pad1A0[0x9D6 - 0x1A0];
    u8 unk9D6;
    u8 pad9D7;
} UnkStruct_0235c238;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Task *data_ov000_0236ac80;
extern void *data_022c4488;
extern void *data_022c448c;
extern u8 data_ov000_023690e8[], data_ov000_023690ec[], data_ov000_02369100[];

void func_ov000_0235bd10(void);
void func_ov000_0235c168(void);
void func_ov000_0235a818(void);
void func_ov000_0235a760(void);
void func_ov000_0235a6a8(void);
void func_ov000_0235a5f8(void);
void func_ov000_0235b484(void);
void func_ov000_0235b644(void);
void func_ov000_0235a9e8(void);
void func_ov000_0235a848(void);
void func_ov000_0235aa4c(void);
void func_ov000_0235a8b0(void);
void func_ov000_0235aab0(void);
void func_ov000_0235a918(void);
void func_ov000_0235ab14(void);
void func_ov000_0235a980(void);
void func_ov000_0235ab48(void);
void func_ov000_0235a9b4(void);

void func_02021310(void *f, void *name, void *heap);
void *func_02021264(void *f, s32 idx);
void func_020212e0(void *f);
s32 func_0203969c(u32 a, s32 b);
s32 func_02039428(u16 a, s32 b);
void func_02031878(void *s, void *res, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_020316f4(void *s);
void *func_020687b8(s32 a, void *name, void *heap);
void func_0206d190(void *a, s32 b, s32 c, s32 d);
s32 func_02039c18(s32 a, s32 b);
void func_0203363c(UnkStruct_Spr *s, void *res, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, u32 g);

static inline BOOL func_ov000_0235c238_IsNormal(void) {
    BOOL r = FALSE;
    if (data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2)
        r = TRUE;
    return r;
}

UnkStruct_0235c238 *func_ov000_0235c238(void) {
    UnkStruct_Task *task;
    UnkStruct_0235c238 *w;
    u32 file[0x5c / 4];
    vu16 zero;
    UnkStruct_0235c238_Res *res;
    UnkStruct_0235c238_ResHdr *hdr;
    s32 a;
    void *mdl;

    task = func_020372d0(func_ov000_0235bd10, func_ov000_0235c168, 0, 0x3000, 2, sizeof(UnkStruct_0235c238));
    data_ov000_0236ac80 = task;
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0235c238));
    w->unk9D6 = 1;
    if (data_022c4560.unk18 & 0x20)
        w->unk9D6 = 2;
    w->unkC = func_ov000_0235a818;
    if (!func_ov000_0235c238_IsNormal()) {
        if (data_022c4560.unk1C == 1)
            w->unkC = func_ov000_0235a760;
        else if (data_022c4560.unk1C == 2)
            w->unkC = func_ov000_0235a6a8;
        else if (data_022c4560.unk1C == 3)
            w->unkC = func_ov000_0235a5f8;
    }
    if (!func_ov000_0235c238_IsNormal() && data_022c4560.unk1C == 7)
        w->unk10 = func_ov000_0235b484;
    else
        w->unk10 = func_ov000_0235b644;
    if (!func_ov000_0235c238_IsNormal()) {
        if (data_022c4560.unk1C == 3) {
            w->unk4 = func_ov000_0235a9e8;
            w->unk8 = func_ov000_0235a848;
        } else if (data_022c4560.unk1C == 2) {
            w->unk4 = func_ov000_0235aa4c;
            w->unk8 = func_ov000_0235a8b0;
        } else if (data_022c4560.unk1C == 1) {
            w->unk4 = func_ov000_0235aab0;
            w->unk8 = func_ov000_0235a918;
        } else {
            w->unk4 = func_ov000_0235ab14;
            w->unk8 = func_ov000_0235a980;
        }
        func_02021310(file, data_ov000_023690e8, data_022c4488);
        res = func_02021264(file, 0);
        func_020212e0(file);
        hdr = (UnkStruct_0235c238_ResHdr *)((u8 *)res + res->unk18);
        a = func_0203969c(hdr->unk2C, 0);
        func_02031878(w->unkA4, res, 0, 0, a, func_02039428(hdr->unk2A, 0), 0, 0);
        w->unkAE = 0x14;
        w->unkAF = 7;
        w->unkC4 = 0x800;
        w->unkC0 = w->unkC4;
        w->unkBC = w->unkC0;
        func_020316f4(w->unkA4);
        w->unk160 |= 0x10;
        mdl = func_020687b8(0, data_ov000_023690ec, data_022c448c);
        func_0206d190(mdl, 0, 2, 0);
        func_0206d190(mdl, 0, 2, 1);
    } else {
        w->unk4 = func_ov000_0235ab48;
        w->unk8 = func_ov000_0235a9b4;
        mdl = func_020687b8(0, data_ov000_02369100, data_022c448c);
        func_0203363c(&w->spr0, mdl, 0, 0, 0, 0, func_02039c18(0, 2), 0, 0x5000200);
        func_0203363c(&w->spr1, mdl, 0, 0, 1, 0, func_02039c18(1, 2), 0, 0x5000600);
        w->unk198 = func_02039c18(0, 2);
        w->unk19C = func_02039c18(1, 2);
        w->spr0.unk44 = 2;
        w->spr1.unk44 = 2;
        w->spr0.unk2C |= 0x4010;
        w->spr1.unk2C |= 0x4010;
        w->spr0.unk46 = 8;
        w->spr1.unk46 = 8;
        func_020698bc(&w->spr0, &w->spr1, 2);
        func_0206d190(mdl, 0, 2, 0);
        func_0206d190(mdl, 0, 2, 1);
    }
    return w;
}
