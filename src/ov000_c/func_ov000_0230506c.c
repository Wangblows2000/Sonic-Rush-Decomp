#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr sprA;
    UnkStruct_Spr sprB;
    UnkStruct_Spr sprC;
    UnkStruct_Spr sprD;
    s16 unk32C;
    s16 unk32E;
} UnkStruct_0230506c;

extern void *data_ov000_02369ba0;
extern void *data_ov000_02369e6c;
extern s16 data_0206e470[];
extern u8 data_ov000_02366778[], data_ov000_02369e64[], data_ov000_02366790[], data_ov000_02369e74[];
extern u8 data_ov000_023667a8[], data_ov000_02369e84[], data_ov000_023667c0[];

void func_ov000_02304ff4(void);
void func_ov000_02304a1c(void);
void func_ov000_02304ba0(void);
void func_020687b8(void *a, void *b, void *c);
void func_ov000_02304fa0(UnkStruct_0230506c *w);

UnkStruct_0230506c *func_ov000_0230506c(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230506c *w;
    vu16 zero;
    s32 idx;

    task = func_020372d0(func_0206c3d0, func_ov000_02304ff4, 0, 0xf6, 2, sizeof(UnkStruct_0230506c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230506c));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366778, 0xffff, data_ov000_02369e64, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x24);
    func_02069918(w, 8);
    func_020698f0(w, 0);
    w->base.unk70 = func_ov000_02304a1c;
    w->base.unk4A += 0x18;
    func_02069d38(&w->sprA, &w->sprB, data_ov000_02366790, 0, data_ov000_02369e64, data_ov000_02369ba0);
    func_02033550(&w->sprA, 1);
    func_02033550(&w->sprB, 1);
    func_0206990c(&w->sprA, &w->sprB, 7);
    func_020698bc(&w->sprA, &w->sprB, 0);
    w->sprA.unk44 = w->base.spr0.unk44;
    w->sprB.unk44 = w->base.spr0.unk44;
    func_02069e68(&w->sprA, &w->sprB, 2, data_ov000_02369e74);
    func_02069d38(&w->sprC, &w->sprD, data_ov000_023667a8, 0, data_ov000_02369e64, data_ov000_02369ba0);
    func_02033550(&w->sprC, 2);
    func_02033550(&w->sprD, 2);
    w->sprC.unk46 = 0x14;
    w->sprD.unk46 = 0x14;
    w->sprC.unk44 = w->base.spr0.unk44;
    w->sprD.unk44 = w->base.spr0.unk44;
    func_02069e68(&w->sprC, &w->sprD, 0x20, data_ov000_02369e84);
    func_020687b8(&data_ov000_02369e6c, data_ov000_023667c0, data_ov000_02369ba0);
    w->base.unk1C8 = data_ov000_02369e6c;
    w->base.unk1B8 = w;
    w->base.unk1D8 = 8;
    w->base.unk1DA = 10;
    w->base.unk1D4 = -0x30;
    w->base.unk1D6 = -0x28;
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = func_ov000_02304ba0;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x100;
    w->base.unk58 |= 0x2000;
    if (!(w->base.unk1E0->unk4 & 2))
        w->base.unk5C &= ~1;
    idx = (s32)((u16)w->base.unkC & 0xffff) >> 4;
    w->unk32C = ((w->base.unk1E8 << 8) + ((data_0206e470[idx * 2] * 0xc000) >> 12)) >> 8;
    if (w->base.unk5C & 1)
        w->unk32C += 0x20;
    else
        w->unk32C -= 0x20;
    if (w->base.unk1E0->unk4 & 1) {
        idx = (s32)((u16)w->base.unkC & 0xffff) >> 4;
        w->unk32E = ((w->base.unk1EC << 8) + ((data_0206e470[(u32)((idx << 1) + 1)] * -0xc000) >> 12)) >> 8;
    } else {
        idx = (s32)((u16)w->base.unkC & 0xffff) >> 4;
        w->unk32E = ((w->base.unk1EC << 8) - ((data_0206e470[(u32)((idx << 1) + 1)] * -0xc000) >> 12)) >> 8;
    }
    func_ov000_02304fa0(w);
    return w;
}
