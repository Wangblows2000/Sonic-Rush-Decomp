#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; s32 x; s32 y; } UnkPos;
s32 Sound_CalcDistanceVolume(s32 x0, s32 y0, s32 x1, s32 y1);
void NNS_SndPlayerSetVolume(void *a, s32 v);
s32 Sound_CalcPan(s32 a, s32 b);
void NNS_SndPlayerSetTrackPan(void *a, s32 b, s32 c);
void Sound_SetPositional(void *snd, UnkPos *a, UnkPos *b) { NNS_SndPlayerSetVolume(snd, Sound_CalcDistanceVolume(a->x, a->y, b->x, b->y)); NNS_SndPlayerSetTrackPan(snd, 0xffff, Sound_CalcPan(a->x, b->x)); }
