#ifndef MAIN_A2_STRM_H
#define MAIN_A2_STRM_H
#include <nitro/types.h>
typedef struct { s32 origin; s32 target; s32 counter; s32 frame; } NNSSndFader;
typedef struct {
    u8 unk0[0x5C];
    u8 file[0x44];
    u32 unkA0;
    u8 unkA4[0x18];
    u8 format;
    u8 unkBD;
    u8 channels;
    u8 unkBF;
    u16 rate;
    u16 timer;
    u8 unkC4[0x20];
    NNSSndFader fader;
    u8 unkF4[0x18];
    s32 unk10C_0 : 1;
    s32 unk10C_1 : 1;
    s32 unk10C_2 : 1;
    s32 unk10C_3 : 1;
    s32 unk10C_4 : 1;
    s32 unk10C_5 : 1;
    s32 unk10C_6 : 1;
    s32 unk110;
    s32 unk114;
    s32 unk118;
    s32 unk11C;
    u8 unk120;
    u8 unk121;
    u8 unk122[6];
    u32 unk128;
    u32 unk12C;
    u32 unk130;
    u32 unk134;
    void *unk138;
    void *unk13C;
    u32 unk140;
    u32 unk144;
    void *handle;
    s32 prio;
    s32 unk150;
    s32 unk154;
    u32 unk158;
} UnkStrm;
#endif
