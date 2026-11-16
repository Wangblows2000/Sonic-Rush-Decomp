#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 type;
    u8 pad18[0x24];
} UnkEnt_022d81c4;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkParam_022d81c4;

typedef struct {
    s32 unk0;
    s32 sel;
    u8 pad8[4];
    s32 unkc;
    UnkEnt_022d81c4 ent[6];
    u8 pad178[0x1a4 - 0x178];
    UnkParam_022d81c4 *param;
} UnkWork_022d81c4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d81c4 *work;
} UnkTask_022d81c4;

UnkTask_022d81c4 *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void TitleMenu_Unk_ov001_022d8b54(void);

#pragma thumb on
void TitleMenu_CreatePageTask(const void *src, UnkParam_022d81c4 *param, s32 n) {
    UnkWork_022d81c4 *w = Task_Create(TitleMenu_Unk_ov001_022d8b54, 0, 0, 0x2000, 0x1000, 0x1a8)->work;
    s32 i;

    MIi_CpuCopy16(src, w->ent, 0x194);
    w->unk0 = param->unk4;
    w->sel = -1;
    w->unkc = 0;
    w->param = param;
    for (i = 0; i < 4; i++) {
        if (w->sel < 0 && w->ent[i].type != 0x14) {
            if (n <= 0) {
                w->sel = i;
                return;
            }
            n--;
        }
    }
}
#pragma thumb reset
