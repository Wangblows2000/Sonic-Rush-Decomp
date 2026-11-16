#include <nitro/types.h>

typedef struct { s32 unk0; u8 pad4[0x18]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

typedef struct {
    void (*fn)(void);
    s32 unk4;
    u8 pad8[0x94];
    s16 unk9C;
    u8 pad9E[2];
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u8 padAC[0xe];
    u8 unkBA;
    u8 unkBB;
} UnkObj_020463bc;
typedef struct {
    u8 pad0[0x274];
    UnkObj_020463bc obj;   /* 0x274 */
    u8 pad330[0x14];
    u32 flags;             /* 0x344 */
    u8 pad348[0x12e8];
    void *unk1630;         /* 0x1630 */
    u8 unk1634[4];         /* 0x1634 */
} UnkWork_020463bc;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void func_0204633c(void);
void Sound_PlaySeq(void *a, s32 b);
void Sound_PlaySeqArc(void *a, void *b, s32 c);
void func_020366f4(void *a, void *b, s32 c);

#pragma thumb on
void func_020463bc(UnkWork_020463bc *w, UnkObj_020463bc *o) {
    o->unk4++;
    if (o->unk4 >= 0x3c) {
        w->flags |= 0x10000;
        o->fn = func_0204633c;
        o->unk4 = 0;
        Sound_PlaySeqArc(NULL, w->unk1630, 3);
        if (data_022c4560.unk0 == 0) {
            if (data_022c4560.unk1C == 7)
                func_020366f4(NULL, w->unk1630, 4);
            else if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2)
                func_020366f4(NULL, w->unk1630, 5);
            else
                func_020366f4(NULL, w->unk1630, 4);
        } else {
            if (data_022c4560.unk1C == 7)
                func_020366f4(NULL, w->unk1630, 5);
            else if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2)
                func_020366f4(NULL, w->unk1630, 7);
            else
                func_020366f4(NULL, w->unk1630, 6);
        }
        o->unkBA = 1;
        o->unkBB = 1;
    }
}
#pragma thumb reset
