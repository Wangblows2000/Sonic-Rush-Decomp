#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x80 - 1];
    void *unk80;
} UnkStruct_02315928_R;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x2E - 2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u8 pad5C[0x1BC - 0x5C];
    UnkStruct_02315928_R *unk1BC;
    u8 pad1C0[0x1DC - 0x1C0];
    void *unk1DC;
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x204 - 0x1E4];
    UnkStruct_02315928_R *unk204;
} UnkStruct_02315928;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02315928_Sys;

extern UnkStruct_02315928_Sys data_022c4560;
extern void *data_ov000_0236ac60;

void func_0201c070(void *p, s32 a);
void func_ov000_023155cc(UnkStruct_02315928 *w);
s16 func_02068db4(s16 a, s32 b);
s16 func_02068df8(s16 a, s32 b, s32 c);
u8 func_02068b04(u8 a, s32 b, s32 c);
void func_ov000_0234f6a0(void *a, void *b);
void func_ov000_023158c0(UnkStruct_02315928 *w);
void func_0204b7ec(void *p, s32 a, s32 b);
void func_02068204(void *a, void *b, void *c);

void func_ov000_02315928(UnkStruct_02315928 *w) {
    UnkStruct_02315928_R *r;
    s16 acc = -8;
    s16 max = 0x1c0;

    if (w->unk58 & 0xf) {
        func_0201c070(w->unk1DC, 0);
        func_ov000_023155cc(w);
        return;
    }
    if (w->unk1E0->unk4 & 1) {
        acc <<= 1;
        max <<= 1;
    }
    w->unk2E = func_02068db4(w->unk2E, 0x10);
    w->unk30 = func_02068df8(w->unk30, acc, max);
    w->unk1 = func_02068b04(w->unk1, 0, 1);
    r = w->unk1BC;
    if (r != NULL && r->unk80 == w && r->unk0 == 1) {
        w->unk204 = r;
        func_ov000_0234f6a0(r, w);
        func_ov000_023158c0(w);
        return;
    }
    if (w->unk58 & 0x2000)
        return;
    if (!(data_022c4560.unk20 & 7))
        func_0204b7ec(w->unk1DC, 0, 0x6c);
    func_02068204(w->unk1DC, w, data_ov000_0236ac60);
}
