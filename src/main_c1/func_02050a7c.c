#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[0xe];
    u16 name[10];  /* 0x1e */
    u16 unk32;
    u8 pad34[0x118 - 0x34];
} UnkWork_02050a7c;
typedef struct { u8 pad0[0x10]; UnkWork_02050a7c *work; } UnkTask_02050a7c;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
typedef struct { u8 pad0[0x18]; u16 unk18; } UnkStruct_02072cb8;
typedef struct {
    u16 name[11];
    u16 flag;
} UnkMsg_02050a7c;
extern UnkTask_02050a7c *data_022c4638;
extern UnkStruct_0207ad9c data_0207ad9c;
extern u16 data_02072cbc[];
extern UnkStruct_02072cb8 gGameLanguage;
extern s32 data_022c4640;

void Task_Destroy(UnkTask_02050a7c *t);
UnkTask_02050a7c *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02050638(void);
void func_02050928(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void func_02051628(s32 a);
void func_0204361c(s32 a, s32 b, s32 c, void *msg, s32 size);

#pragma thumb on
UnkWork_02050a7c *func_02050a7c(s32 target) {
    UnkWork_02050a7c *w;
    vu16 zero;
    s32 i;
    u16 n;
    UnkMsg_02050a7c msg;

    if (data_022c4638 != NULL) {
        Task_Destroy(data_022c4638);
        data_022c4638 = NULL;
    }
    data_022c4638 = Task_Create(func_02050638, func_02050928, 0, 0x1000, 0, sizeof(UnkWork_02050a7c));
    w = data_022c4638->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_02050a7c));
    w->unk0 = 0;
    w->unk4 = 2;
    data_0207ad9c.unk0 = w->unk0;
    data_0207ad9c.unk2 = w->unk4;
    MIi_CpuCopy16(data_02072cbc, w->name, 0x14);
    n = gGameLanguage.unk18;
    for (i = n; i < 10; i++)
        w->name[i] = ' ';
    w->unk32 = i = n;
    func_02051628(0);
    MIi_CpuCopy16(w->name, msg.name, 0x16);
    msg.flag = 1;
    func_0204361c(0, 1, target, &msg, sizeof(UnkMsg_02050a7c));
    w->unkC = 1;
    data_022c4640 = 0;
    return w;
}
#pragma thumb reset
