#include <nitro/types.h>

typedef struct UnkStruct_02318b08_3 {
    u8 pad[0x330];
    u32 unk330;
} UnkStruct_02318b08_3;

typedef struct UnkStruct_02318b08_2 {
    s16 unk0;
    u16 unk2;
    u8 pad[4];
    void *unk8;
} UnkStruct_02318b08_2;

typedef struct UnkStruct_02318b08 {
    u8 pad[0x10];
    UnkStruct_02318b08_2 *unk10;
} UnkStruct_02318b08;

extern UnkStruct_02318b08_3 *gPlayers;
extern u16 gSwitchTimers[];

void NNS_SndPlayerStopSeq(void *p, s32 a);
void NNS_SndHandleReleaseSeq(void *p);
void Sound_FreeHandle(void *p);
void func_ov000_0233e5c8(s32 a, s32 b);

void SwitchTimer_Destructor(UnkStruct_02318b08 *s) {
    UnkStruct_02318b08_2 *p = s->unk10;
    NNS_SndPlayerStopSeq(p->unk8, 0);
    if (p->unk8 != NULL)
        NNS_SndHandleReleaseSeq(p->unk8);
    if (p->unk8 != NULL)
        Sound_FreeHandle(p->unk8);
    if (p->unk2 != 0) {
        if (gPlayers->unk330 & 0x8000)
            func_ov000_0233e5c8(3, 8);
        gPlayers->unk330 &= ~0x4000;
    }
    gSwitchTimers[p->unk0] = 0;
}
