#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkc;
} UnkWork_022d7968;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d7968 *work;
} UnkTask_022d7968;

typedef struct {
    u8 pad[8];
    s16 bright;
} UnkBright_022d7968;

extern UnkBright_022d7968 data_02087d44;
extern UnkBright_022d7968 data_0208926c;

void Task_DestroyRange(u16 lo, u16 hi);
UnkTask_022d7968 *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void TitleMenuFadeTask_Main(void);

#pragma thumb on
void TitleMenu_Unk_ov001_022d7968(s32 a, s32 b) {
    UnkWork_022d7968 *w;

    Task_DestroyRange(0x4000, 0x4000);
    if (a != 0) {
        w = Task_Create(TitleMenuFadeTask_Main, 0, 0, 0x4000, 0x1000, 0x10)->work;
        {
            vu16 z = 0;
            MIi_CpuClear16(z, w, 0x10);
        }
        w->unk0 = 0;
        w->unk2 = 0x10;
        w->unk4 = 0;
        w->unk8 = a;
        w->unkc = b;
        data_02087d44.bright = 0;
        data_0208926c.bright = data_02087d44.bright;
    }
}
#pragma thumb reset
