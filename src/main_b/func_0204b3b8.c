#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern void (*data_0207425c[][3])(void);
extern u32 data_ov000_02369ba0;
void NNS_SndStopSoundAll(void);
void NNS_SndStopChannelAll(void);
void func_ov000_0233bc3c(void);
void func_ov000_023357e0(void);
void func_0204b3b8(void) { NNS_SndStopSoundAll(); NNS_SndStopChannelAll(); data_0207425c[data_022c4560.unk1C][data_022c4560.unk1D](); func_ov000_0233bc3c(); func_ov000_023357e0(); data_ov000_02369ba0 = 0; }
