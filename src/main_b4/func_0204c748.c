#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x18];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern u8 data_022c44f4[];
extern u8 data_022c44f0[];
extern void *data_02250464;
extern u16 data_02074850[][3][2];
extern u16 data_02074852[][3][2];
extern u8 gBGMVolume;

void NNS_SndHandleInit(void *p);
void NNS_SndPlayerStopSeq(void *p, s32 a);
void Sound_PlaySeq(void *p, u16 a);
void NNS_SndArcPlayerStartSeq(void *p, u16 a);
void NNS_SndPlayerSetVolume(void *p, u8 a);
void func_0204c66c(s32 a);

void func_0204c748(s32 a) {
    NNS_SndHandleInit(data_022c44f4);
    NNS_SndHandleInit(data_022c44f0);
    if (data_022c4560.unk1c >= 9 || data_022c4560.unk1d >= 2) {
        NNS_SndPlayerStopSeq(&data_02250464, 0);
        Sound_PlaySeq(&data_02250464, data_02074850[data_022c4560.unk1c][data_022c4560.unk1d][0]);
        return;
    }
    NNS_SndPlayerStopSeq(data_022c44f4, 0);
    NNS_SndPlayerStopSeq(data_022c44f0, 0);
    if (data_022c4560.unk0 == 0) {
        NNS_SndArcPlayerStartSeq(data_022c44f4, data_02074850[data_022c4560.unk1c][data_022c4560.unk1d][0]);
        NNS_SndPlayerSetVolume(data_022c44f4, gBGMVolume);
        NNS_SndArcPlayerStartSeq(data_022c44f0, data_02074852[data_022c4560.unk1c][data_022c4560.unk1d][0]);
        NNS_SndPlayerSetVolume(data_022c44f0, gBGMVolume);
    } else {
        NNS_SndArcPlayerStartSeq(data_022c44f4, data_02074852[data_022c4560.unk1c][data_022c4560.unk1d][0]);
        NNS_SndPlayerSetVolume(data_022c44f4, gBGMVolume);
        NNS_SndArcPlayerStartSeq(data_022c44f0, data_02074850[data_022c4560.unk1c][data_022c4560.unk1d][0]);
        NNS_SndPlayerSetVolume(data_022c44f0, gBGMVolume);
    }
    func_0204c66c(a);
}
