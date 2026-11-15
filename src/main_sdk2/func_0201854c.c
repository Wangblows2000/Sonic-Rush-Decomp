#include <nitro/types.h>
#include <main_sdk2/wh.h>

void func_020197bc(UnkCallback_wh *arg);
s32 func_02011598(void (*cb)(UnkCallback_wh *), u16 aid);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_0201a934(u16 aid);

static inline void func_0201854c_Leave(u16 aid)
{
    if (data_02080f1c->unk1526[aid - 1] != -1) {
        u8 k = data_02080f1c->unk1526[aid - 1];
        ((UnkStruct_02080f1c *)((u8 *)data_02080f1c + k * sizeof(UnkEntry1788_wh)))->unk1788[0].unk5c6 &= ~(1 << aid);
        ((UnkStruct_02080f1c *)((u8 *)data_02080f1c + k * sizeof(UnkEntry1788_wh)))->unk1788[0].unk5c8 |= 1 << aid;
        data_02080f1c->unk1526[aid - 1] = -1;
        ((UnkStruct_02080f1c *)((u8 *)data_02080f1c + k * sizeof(UnkEntry1788_wh)))->unk1788[0].unk5c4 &= ~(1 << aid);
    }
}

void func_0201854c(u16 aid)
{
    s32 i;
    u32 bit;

    func_02011598(func_020197bc, aid);
    if (aid == 0 || aid >= 16) {
        return;
    }
    i = aid - 1;
    data_02080f1c->unk148a[i] = 0;
    MI_CpuFill8(&data_02080f1c->unk14a8[i], 0, 4);
    MI_CpuFill8(&data_02080f1c->childInit[i], 0, sizeof(UnkInitParam_wh));
    func_0201a934(aid);
    data_02080f1c->unk1754[i] = 0;
    func_0201854c_Leave(aid);
    bit = 1 << aid;
    if (data_02080f1c->unk1536 & bit) {
        data_02080f1c->unk1535--;
        data_02080f1c->unk1536 &= ~bit;
    }
    data_02080f1c->unk14e8[i] = 0;
}
