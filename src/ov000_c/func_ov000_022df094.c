#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    UnkStruct_Vec unk48;
    u8 pad54[0x138 - 0x54];
} UnkStruct_022df094_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022df094_Sub unk20C;
    u8 pad344[0x34C - 0x344];
    u16 unk34C;
    u8 pad34E[0x3C8 - 0x34E];
} UnkStruct_022df094;

typedef struct {
    u8 pad0[0xBAC];
    UnkStruct_Vec unkBAC;
    u8 padBB8[0xBDC - 0xBB8];
    s32 unkBDC;
} UnkStruct_02369b2c;

extern UnkStruct_02369b2c *data_ov000_02369b2c;
extern void *data_ov000_02369b30[];

void func_ov000_022dc504(void);
void func_ov000_022dc528(void);
void func_ov000_022dc568(void);
void func_ov000_022dc328(void);
void MI_CpuFill8(void *p, s32 v, u32 n);
void func_02031c1c(void *s, s32 a);
void func_02031bd0(void *s, void *res, s32 idx);
void func_ov000_022db7ec(UnkStruct_Vec *v, UnkStruct_022df094 *w);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022dc2ec(UnkStruct_022df094 *w);

UnkStruct_022df094 *func_ov000_022df094(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_02369b2c *boss = data_ov000_02369b2c;
    UnkStruct_022df094 *w;
    UnkStruct_022df094_Sub *sub;

    if (boss->unkBDC >= 1)
        return NULL;
    boss->unkBDC++;
    w = func_020372d0(func_0206c3d0, func_ov000_022dc504, 0, 0x504, 2, sizeof(UnkStruct_022df094))->work;
    MI_CpuFill8(w, 0, sizeof(UnkStruct_022df094));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022dc528;
    w->base.unk68 = func_ov000_022dc568;
    w->base.unk78 = func_ov000_022dc328;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0xa300;
    sub = &w->unk20C;
    func_02031c1c(sub, 0);
    func_02031bd0(sub, data_ov000_02369b30[5], 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unk48 = boss->unkBAC;
    func_ov000_022db7ec(&sub->unk48, w);
    w->unk34C = 0;
    func_02036754(0, 0, 0x8a);
    func_ov000_022dc2ec(w);
    return w;
}
