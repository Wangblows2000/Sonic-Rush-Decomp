#include <nitro/types.h>

typedef struct {
    s8 s[6][20];
} UnkStr_0204dce8;

typedef struct {
    s8 w[24];
} UnkWidth_0204dce8;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_0204dce8;

extern UnkStr_0204dce8 data_0207a144;
extern UnkWidth_0204dce8 data_0207a0b4;
extern u8 gGameLanguage;
extern char data_0207a1d8[];
extern void *gSoundHeap;

UnkTask_0204dce8 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_0204dbb8(void);
void func_0204dccc(void);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void BrightnessFade_Create(s32 a, s32 b);
void GameOver_Create(u16 c, s32 x, s32 a, s32 b, s32 i, s32 cur);
void func_02036854(void *p);
void NNS_SndArcLoadGroup(s32 a, void *b);
void Sound_PlaySeq(s32 a, s32 b);
void ScreenShake_Create(void);

static inline s32 LangIdx_0204dce8(u8 lang) {
    s32 l;
    switch (lang) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        l = lang;
        break;
    default:
        l = 1;
        break;
    }
    return l;
}

void func_0204dce8(void) {
    UnkStr_0204dce8 tbl;
    UnkWidth_0204dce8 wt;
    UnkTask_0204dce8 *t;
    u16 i;
    u32 total;
    s16 x;
    s16 cur;
    u8 lang;

    cur = 0;
    tbl = data_0207a144;
    wt = data_0207a0b4;
    t = Task_Create(func_0204dbb8, func_0204dccc, 7, 0x6100, 3, 4);
    if (t == (UnkTask_0204dce8 *)OS_GetArenaLo(0))
        return;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, t->work, 4);
    }
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1000;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1000;
    *(vu16 *)0x5000000 = 0;
    *(vu16 *)0x5000400 = 0;
    BrightnessFade_Create(0x42, 0x100);
    total = 0;
    lang = gGameLanguage;
    for (i = 0;; i++) {
        if (tbl.s[LangIdx_0204dce8(lang)][i] == -2)
            break;
        if (tbl.s[LangIdx_0204dce8(lang)][i] == -1)
            total += wt.w[0x17];
        else
            total += wt.w[tbl.s[LangIdx_0204dce8(lang)][i]];
    }
    x = (0x100 - total) >> 1;
    if (x < 0)
        x = 0;
    for (i = 0;; i++) {
        if (tbl.s[LangIdx_0204dce8(lang)][i] == -2)
            break;
        if (tbl.s[LangIdx_0204dce8(lang)][i] == -1) {
            cur += wt.w[0x17];
        } else {
            GameOver_Create(tbl.s[LangIdx_0204dce8(lang)][i], x, 0x108, 0x68, i, cur);
            cur += wt.w[tbl.s[LangIdx_0204dce8(lang)][i]];
        }
    }
    func_02036854(data_0207a1d8);
    NNS_SndArcLoadGroup(0xb, gSoundHeap);
    Sound_PlaySeq(0, 0x14);
    ScreenShake_Create();
}
