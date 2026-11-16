#include <nitro/types.h>

typedef struct {
    u8 pad0[0x32C];
    u32 unk32C;
} UnkStruct_02342a74_Pl;

typedef struct {
    void *spr;
    u16 state;
    u8 pad6[2];
    UnkStruct_02342a74_Pl *pl;
} UnkStruct_02342a74;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02342a74 *work;
} UnkStruct_02342a74_Task;

extern UnkStruct_02342a74_Task *data_ov000_0236ac34;

UnkStruct_02342a74_Task *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void *func_02006abc(u32 arg);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void *func_0203663c(void);
void func_0201c070(void *spr, s32 a);
void func_0204c598(u8 a, s32 b, u32 c);
void func_0204b860(void *spr, u16 a, s32 b, s32 c);
void func_0201be10(void *spr, s32 a);
void func_ov000_023428dc(void);
void func_ov000_02342a34(void);

void func_ov000_02342a74(UnkStruct_02342a74_Pl *pl, u16 state) {
    UnkStruct_02342a74_Task *t;
    UnkStruct_02342a74 *w;
    vu16 zero;

    if (data_ov000_0236ac34 == NULL) {
        t = func_020372d0(func_ov000_023428dc, func_ov000_02342a34, 7, 0xA00, 4, sizeof(UnkStruct_02342a74));
        if (t == func_02006abc(0))
            return;
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkStruct_02342a74));
        data_ov000_0236ac34 = t;
        w->spr = func_0203663c();
    } else {
        w = data_ov000_0236ac34->work;
    }
    w->pl = pl;
    if (w->state != 0)
        func_0201c070(w->spr, 0);
    w->state = state;
    func_0204c598(0, 0, w->pl->unk32C & 0x4000000);
    func_0204b860(w->spr, w->state, 100, -1);
    func_0201be10(w->spr, 0x50);
}
