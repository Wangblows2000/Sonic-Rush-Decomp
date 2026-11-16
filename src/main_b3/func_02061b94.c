#include <nitro/types.h>

typedef struct { u32 unk0; s32 unk4; s16 v[6]; } UnkEnt_02077770;
typedef struct {
    u32 unk0;
    u8 pad4[8];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x10];
    u32 unk34;
} UnkParam_02061b94;
typedef struct { u8 pad0[4]; void *unk4; } UnkSnd_02061b94;
typedef struct { UnkSnd_02061b94 *snd; } UnkRet_02060384;
typedef struct { u8 pad0[0x18]; s32 unk18; } UnkStruct_02076e58;
extern UnkEnt_02077770 data_02077770[];
extern UnkStruct_02076e58 data_02076e58;

UnkRet_02060384 *TalkDemo_GetEntry(void);
void Sound_PlaySeqArc(void *a, void *b, s32 c);
void func_02061ee4(void);
void func_02061a08(UnkParam_02061b94 *p);

#pragma thumb on
BOOL func_02061b94(UnkParam_02061b94 *p, s32 id) {
    UnkEnt_02077770 *e = &data_02077770[id];

    p->unk34 = 0;
    if (id == 1) {
        p->unk0 = 0;
        return TRUE;
    }
    p->unk0 = e->unk0;
    p->unkC = e->v[0] << 8;
    p->unk10 = e->v[1] << 8;
    p->unk14 = e->v[2] << 8;
    p->unk18 = e->v[3] << 8;
    p->unk1C = e->v[4];
    p->unk20 = e->v[5];
    p->unk34 |= 4;
    switch (id) {
    case 9:
    case 10:
        p->unk34 |= 1;
        break;
    case 11:
    case 12: {
        UnkRet_02060384 *r;
        p->unk34 |= 0x20;
        r = TalkDemo_GetEntry();
        if (r->snd != NULL)
            Sound_PlaySeqArc(NULL, r->snd->unk4, data_02076e58.unk18);
        break;
    }
    case 13:
        p->unk34 |= 0x40;
        func_02061ee4();
        break;
    }
    func_02061a08(p);
    return TRUE;
}
#pragma thumb reset
