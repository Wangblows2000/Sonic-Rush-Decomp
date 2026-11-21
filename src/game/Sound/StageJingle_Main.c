#include <nitro/types.h>

typedef struct {
    u8 pad0[0x32C];
    u32 unk32C;
    u8 pad330[0x3B4 - 0x330];
    s16 unk3B4;
    u8 pad3B6[0x3CA - 0x3B6];
    s16 unk3CA;
} UnkStruct_023428dc_Pl;

typedef struct {
    void *spr;
    u16 state;
    u8 pad6[2];
    UnkStruct_023428dc_Pl *pl;
} UnkStruct_023428dc;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_023428dc *work;
} UnkStruct_023428dc_T;

extern UnkStruct_023428dc_T *gTaskCurrent;
extern u8 gBGMVolume;
extern u32 gSystemFlags;

void NNS_SndPlayerStopSeq(void *spr, s32 a);
void NNS_SndHandleReleaseSeq(void *spr);
void NNS_SndPlayerPause(void *spr, s32 a);
void Stage_FadeBGMLayer(u8 a, s32 b, u32 c);
void Task_Destroy(void *task);

void StageJingle_Main(void) {
    UnkStruct_023428dc *w = gTaskCurrent->work;

    switch (w->state) {
    case 0x16:
        if (w->pl->unk3B4 == 0x5A) {
            NNS_SndPlayerStopSeq(w->spr, 0x5A);
            NNS_SndHandleReleaseSeq(w->spr);
            Stage_FadeBGMLayer(gBGMVolume, 0x5A, w->pl->unk32C & 0x4000000);
            Task_Destroy(gTaskCurrent);
        } else if (w->pl->unk3B4 == 0) {
            NNS_SndPlayerStopSeq(w->spr, 0);
            Stage_FadeBGMLayer(gBGMVolume, 0, w->pl->unk32C & 0x4000000);
            Task_Destroy(gTaskCurrent);
        }
        break;
    case 0x17:
        if (gSystemFlags & 8) {
            NNS_SndPlayerPause(w->spr, 1);
        } else {
            NNS_SndPlayerPause(w->spr, 0);
        }
        if (w->pl->unk3CA == 0) {
            NNS_SndPlayerStopSeq(w->spr, 0);
            Stage_FadeBGMLayer(gBGMVolume, 0x3C, w->pl->unk32C & 0x4000000);
            Task_Destroy(gTaskCurrent);
        }
        break;
    }
}
