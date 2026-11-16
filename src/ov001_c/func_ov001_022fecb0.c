#include <nitro/types.h>

typedef struct UnkStruct_022fecb0_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022fecb0_t;

typedef struct UnkStruct_022fecb0 {
    u32 flags;
    s32 unk4;
    UnkStruct_022fecb0_t *task8;
    UnkStruct_022fecb0_t *taskC;
    UnkStruct_022fecb0_t *task10;
    u8 pad14[0x40 - 0x14];
    s32 unk40;
    s32 unk44;
    u8 pad48[4];
    s32 unk4C;
    u8 pad50[0x318 - 0x50];
    s32 unk318;
} UnkStruct_022fecb0;

extern UnkStruct_022fecb0 *data_ov001_0231f52c;
extern s16 gDisplaySub[];
extern s16 gDisplayMain[];
extern u8 data_02074960[];
extern u8 data_020748bc[];
extern u8 data_02074ba4[];

void func_ov001_022ff3a4(void);
void func_ov001_02302370(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 b);
void func_02022270(u32 index, u32 value);
void func_02022284(u32 idx, s32 x, s32 y, s32 z);
void func_02004084(const void *src);
void func_0205ef48(void);
void Sound_PlaySeq(u32 *p, u32 a);

void func_ov001_022fecb0(void) {
    u16 col;

    *(vu16 *)0x4000304 |= 0x8000;
    data_ov001_0231f52c->flags |= 2;
    data_ov001_0231f52c->flags &= ~0x74;
    data_ov001_0231f52c->task8->fn = func_ov001_022ff3a4;
    data_ov001_0231f52c->taskC->fn = 0;
    data_ov001_0231f52c->task10->fn = 0;
    func_ov001_02302370();
    data_ov001_0231f52c->unk4C = 0x32;
    data_ov001_0231f52c->unk4 = 0;
    data_ov001_0231f52c->unk44 = 0;
    data_ov001_0231f52c->unk318 = 0;
    gDisplaySub[0x24] = -0x10;
    gDisplayMain[0x24] = gDisplaySub[0x24];
    GXx_SetMasterBrightness_((vu16 *)0x400006c, gDisplayMain[0x24]);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, gDisplaySub[0x24]);
    *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 2;
    *(vu16 *)0x400000c = *(vu16 *)0x400000c & ~3;
    col = *(u16 *)(data_02074960 + 0x9c);
    func_02022270(0, col);
    func_02022270(1, col);
    func_02022270(2, col);
    func_02022284(0, *(s16 *)(data_020748bc + 0x90), *(s16 *)(data_020748bc + 0x92), *(s16 *)(data_020748bc + 0x94));
    func_02022284(1, *(s16 *)(data_020748bc + 0x96), *(s16 *)(data_020748bc + 0x98), *(s16 *)(data_020748bc + 0x9a));
    func_02022284(2, *(s16 *)(data_020748bc + 0x9c), *(s16 *)(data_020748bc + 0x9e), *(s16 *)(data_020748bc + 0xa0));
    func_02004084(data_02074ba4);
    func_0205ef48();
    if (data_ov001_0231f52c->unk40 < 3)
        Sound_PlaySeq(0, 0x14);
    else
        Sound_PlaySeq(0, 0x15);
}
