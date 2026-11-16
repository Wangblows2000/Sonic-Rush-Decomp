#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[0xe];
    u16 name[10];  /* 0x1e */
    u16 unk32;
    u8 pad34[0x4c - 0x34];
} UnkWork_02050b58;
typedef struct { u8 pad0[0x10]; UnkWork_02050b58 *work; } UnkTask_02050b58;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
typedef struct { u8 pad0[0x18]; u16 unk18; } UnkStruct_02072cb8;
typedef struct {
    u16 name[11];
    u16 flag;
} UnkMsg_02050b58;
extern UnkTask_02050b58 *data_022c4638;
extern UnkStruct_0207ad9c data_0207ad9c;
extern u16 data_02072cbc[];
extern UnkStruct_02072cb8 data_02072cb8;
extern s32 data_022c4640;

void func_020371c8(UnkTask_02050b58 *t);
UnkTask_02050b58 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02050764(void);
void func_02050964(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void func_02051628(s32 a);
void func_0204390c(s32 a, s32 b, s32 c, void *msg, s32 size);

#pragma thumb on
UnkWork_02050b58 *func_02050b58(s32 target) {
    UnkWork_02050b58 *w;
    vu16 zero;
    s32 i;
    u16 n;
    UnkMsg_02050b58 msg;

    if (data_022c4638 != NULL) {
        func_020371c8(data_022c4638);
        data_022c4638 = NULL;
    }
    data_022c4638 = func_020372d0(func_02050764, func_02050964, 0, 0x1000, 0, sizeof(UnkWork_02050b58));
    w = data_022c4638->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_02050b58));
    w->unk0 = 0;
    w->unk4 = 0;
    data_0207ad9c.unk0 = w->unk0;
    data_0207ad9c.unk2 = w->unk4;
    MIi_CpuCopy16(data_02072cbc, w->name, 0x14);
    n = data_02072cb8.unk18;
    for (i = n; i < 10; i++)
        w->name[i] = ' ';
    w->unk32 = i = n;
    func_02051628(0);
    MIi_CpuCopy16(w->name, msg.name, 0x16);
    msg.flag = 1;
    func_0204390c(0, 1, target, &msg, sizeof(UnkMsg_02050b58));
    w->unkC = 1;
    data_022c4640 = 0;
    return w;
}
#pragma thumb reset
