#include <nitro/types.h>
#include <main_sdk2/wh.h>

void func_02016928(u16 aid, u32 type, void *arg);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_0201a934(u16 aid);
void func_02015e6c(void *arg);
void func_02015790(void);
void func_020180a4(void *a, void *b, u16 c, u16 d);
u32 func_020183d4(void);
u32 func_020183c0(void);
u8 func_02018360(void);
void func_02017f38(u32 a, u32 b, u8 c);
void func_020156d8(u32 a, u32 b);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);

static inline void func_0201640c_Leave(u16 aid)
{
    if (data_02080f1c->unk1526[aid - 1] != -1) {
        u8 k = data_02080f1c->unk1526[aid - 1];
        ((UnkStruct_02080f1c *)((u8 *)data_02080f1c + k * sizeof(UnkEntry1788_wh)))->unk1788[0].unk5c6 &= ~(1 << aid);
        ((UnkStruct_02080f1c *)((u8 *)data_02080f1c + k * sizeof(UnkEntry1788_wh)))->unk1788[0].unk5c8 |= 1 << aid;
        data_02080f1c->unk1526[aid - 1] = -1;
        ((UnkStruct_02080f1c *)((u8 *)data_02080f1c + k * sizeof(UnkEntry1788_wh)))->unk1788[0].unk5c4 &= ~(1 << aid);
    }
}

void func_0201640c(s32 code, UnkCallback_wh *arg)
{
    switch (code) {
    case 0x15:
        func_02016928(0, 1, arg);
        break;
    case 0:
        if (arg->aid != 0 && arg->aid < 16) {
            func_02016928(arg->aid, 2, arg);
        }
        break;
    case 1:
        if (arg->aid != 0 && arg->aid < 16) {
            data_02080f1c->unk148a[arg->aid - 1] = 0;
            MI_CpuFill8(&data_02080f1c->unk14a8[arg->aid - 1], 0, 4);
            MI_CpuFill8(&data_02080f1c->childInit[arg->aid - 1], 0, sizeof(UnkInitParam_wh));
            func_0201a934(arg->aid);
            data_02080f1c->unk1754[arg->aid - 1] = 0;
            func_0201640c_Leave(arg->aid);
            if (data_02080f1c->unk1536 & (1 << arg->aid)) {
                data_02080f1c->unk1535--;
                data_02080f1c->unk1536 &= ~(1 << arg->aid);
            }
            if (data_02080f1c->unk14e8[arg->aid - 1] == 8) {
                func_02016928(arg->aid, 9, NULL);
            }
            func_02016928(arg->aid, 3, arg);
            data_02080f1c->unk14e8[arg->aid - 1] = 0;
        }
        break;
    case 3:
        func_02015e6c(arg);
        break;
    case 0x19:
        func_02015790();
        break;
    case 0x1c: {
        u8 i;
        for (i = 0; i < 16; i++) {
            if (data_02080f1c->unk1788[i].unk5ca != 0 && data_02080f1c->unk1788[i].unk5c8 != 0) {
                func_020180a4(data_02080f1c->unk1788[i].unkE4, data_02080f1c->childInit,
                              data_02080f1c->unk1788[i].unk5c6, data_02080f1c->unk1788[i].unk5c8);
                data_02080f1c->unk1788[i].unk5c8 = 0;
            }
        }
        func_02017f38(func_020183d4(), func_020183c0(), func_02018360());
        break;
    }
    case 0xff:
        switch (arg->errcode) {
        case 1:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
            func_020156d8(0, 9);
            break;
        case 0:
        case 2:
        case 3:
        case 7:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            func_020156d8(0, 8);
            break;
        }
        break;
    case 0x100:
        switch (arg->apiid) {
        case 0:
        case 7:
        case 8:
        case 13:
        case 14:
        case 15:
        case 17:
        case 18:
        case 21:
        case 25:
        case 29:
            func_020156d8(0, 9);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
        case 10:
        case 11:
        case 12:
        case 16:
        case 19:
        case 20:
        case 22:
        case 23:
        case 24:
        case 26:
        case 27:
        case 28:
        default:
            func_020156d8(0, 8);
            break;
        }
        break;
    case 0x11:
        break;
    }
    if (code == 0x11) {
        void (*cb)(u32, u32, u32) = data_02080f1c->unk14e4;
        {
            vu32 fill = 0;
            MIi_CpuClearFast(fill, data_02080f1c, 0x7d00);
        }
        data_02080f1c = NULL;
        if (cb != NULL) {
            cb(0, 12, 0);
        }
    }
}
