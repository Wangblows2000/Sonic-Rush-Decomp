#include <nitro/types.h>

typedef struct {
    u8 pad0[0x32C];
    u32 unk32C;
} UnkStruct_02342a74_Pl;

typedef struct {
    void *spr;
    u16 state;
    u8 pad6[2];
    UnkStruct_02342a74_Pl *pl;
} UnkStruct_02342a74;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02342a74 *work;
} UnkStruct_02342a74_Task;

extern UnkStruct_02342a74_Task *data_ov000_0236ac34;

UnkStruct_02342a74_Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void *OS_GetArenaLo(u32 arg);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void *Sound_AllocHandle(void);
void NNS_SndPlayerStopSeq(void *spr, s32 a);
void Stage_FadeBGMLayer(u8 a, s32 b, u32 c);
void Sound_PlayBGMEx(void *spr, u16 a, s32 b, s32 c);
void NNS_SndPlayerSetChannelPriority(void *spr, s32 a);
void func_ov000_023428dc(void);
void func_ov000_02342a34(void);

void func_ov000_02342a74(UnkStruct_02342a74_Pl *pl, u16 state) {
    UnkStruct_02342a74_Task *t;
    UnkStruct_02342a74 *w;
    vu16 zero;

    if (data_ov000_0236ac34 == NULL) {
        t = Task_Create(func_ov000_023428dc, func_ov000_02342a34, 7, 0xA00, 4, sizeof(UnkStruct_02342a74));
        if (t == OS_GetArenaLo(0))
            return;
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkStruct_02342a74));
        data_ov000_0236ac34 = t;
        w->spr = Sound_AllocHandle();
    } else {
        w = data_ov000_0236ac34->work;
    }
    w->pl = pl;
    if (w->state != 0)
        NNS_SndPlayerStopSeq(w->spr, 0);
    w->state = state;
    Stage_FadeBGMLayer(0, 0, w->pl->unk32C & 0x4000000);
    Sound_PlayBGMEx(w->spr, w->state, 100, -1);
    NNS_SndPlayerSetChannelPriority(w->spr, 0x50);
}
