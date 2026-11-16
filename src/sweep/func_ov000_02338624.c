#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 hdrOfs;
} UnkStruct_02338624_Res;

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 padA[0x44 - 0xA];
    u16 unk44;
    u8 pad46[0x48 - 0x46];
    UnkStruct_02338624_Res *res;
    u32 idx;
} UnkStruct_02338624;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02338624 *work;
} UnkStruct_02338624_Task;

extern UnkStruct_02338624_Task *data_ov000_0236aaac;
extern u8 data_ov000_023687e8[];
extern s16 data_ov000_02364d8c[];

UnkStruct_02338624_Task *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
UnkStruct_02338624_Res *func_02033fbc(void *path, s32 a);
void *func_02039c18(u32 a, u32 b);
void func_0203363c(void *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
u16 func_0206d2d0(void *res, u32 a, s32 b);
void func_ov000_02338504(void);
void func_ov000_02338538(void);

void func_ov000_02338624(u32 idx) {
    UnkStruct_02338624 *w;
    vu16 zero;

    data_ov000_0236aaac = func_020372d0(func_ov000_02338504, func_ov000_02338538, 0, 0x3800, 3, sizeof(UnkStruct_02338624));
    w = data_ov000_0236aaac->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02338624));
    w->idx = idx;
    w->res = func_02033fbc(data_ov000_023687e8, -1);
    func_0203363c(w, w->res, (u16)idx, 0x34, 0, 0,
                  func_02039c18(0, *(u16 *)((u8 *)w->res + w->res->hdrOfs + 0x1E + (u16)idx * 0x14)), 0, 0x5000200);
    w->unk44 = func_0206d2d0(w->res, w->unk8, data_ov000_02364d8c[idx]);
}
