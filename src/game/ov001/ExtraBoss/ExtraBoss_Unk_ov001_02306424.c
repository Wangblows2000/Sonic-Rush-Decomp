#include <nitro/types.h>

typedef struct UnkStruct_02306424_b {
    u8 pad0[0x74];
    u16 f74[0x22];
    u16 fB8[0x20];
    u8 pad[0x180 - 0xf8];
} UnkStruct_02306424_b;

typedef struct UnkStruct_02306424 {
    u32 unk0;
    void *task4;
    u8 pad8[0x18 - 8];
    u8 bg[4][0x40];
    u8 pad118[0x2d8 - 0x118];
    u16 blk[2][0xc0];
} UnkStruct_02306424;

typedef struct UnkStruct_02306424_g {
    u8 pad0[0x3c];
    s32 unk3C;
} UnkStruct_02306424_g;

extern UnkStruct_02306424 *data_ov001_0231f5a8;
extern s16 gDisplaySub[];

UnkStruct_02306424_g *ExtraBoss_GetWork(void);
void Background_Draw(void *a);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void ExtraBoss_Unk_ov001_023062f0(void);
void ExtraBoss_Unk_ov001_02306244(void);

void ExtraBoss_Unk_ov001_02306424(void) {
    s32 k = ExtraBoss_GetWork()->unk3C;

    data_ov001_0231f5a8->unk0 = 0;
    Background_Draw(data_ov001_0231f5a8->bg[k]);
    {
        vu32 f = -1;
        MIi_CpuClearFast(f, (void *)0x6204000, 0x20);
    }
    {
        vu32 f = 0;
        MIi_CpuClearFast(f, (void *)0x6200000, 0x600);
    }
    *(vu32 *)0x4001010 = 0;
    gDisplaySub[0] = 0;
    gDisplaySub[1] = 0;
    *(vu32 *)0x4001014 = 0x80;
    gDisplaySub[3] = 0;
    gDisplaySub[2] = 0x80;
    {
        vu16 f = 0x80;
        MIi_CpuClear16(f, data_ov001_0231f5a8->blk, 0x300);
    }
    {
        vu16 f = 0x100;
        MIi_CpuClear16(f, (u16 *)((u8 *)data_ov001_0231f5a8->blk[0] + 0x74), 0x40);
    }
    {
        vu16 f = 0;
        MIi_CpuClear16(f, (u16 *)((u8 *)data_ov001_0231f5a8->blk[0] + 0xb8), 0x40);
    }
    {
        vu16 f = 0x100;
        MIi_CpuClear16(f, (u16 *)((u8 *)data_ov001_0231f5a8->blk[1] + 0x74), 0x40);
    }
    {
        vu16 f = 0;
        MIi_CpuClear16(f, (u16 *)((u8 *)data_ov001_0231f5a8->blk[1] + 0xb8), 0x40);
    }
    data_ov001_0231f5a8->task4 = Task_Create(ExtraBoss_Unk_ov001_023062f0, 0, 0, 0x3000, 2, 0);
    ExtraBoss_Unk_ov001_02306244();
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1300;
}
