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
extern u8 data_02072cb8;
extern char data_0207a1d8[];
extern void *data_02250458;

UnkTask_0204dce8 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_0204dbb8(void);
void func_0204dccc(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void func_02043c7c(s32 a, s32 b);
void func_0204da44(u16 c, s32 x, s32 a, s32 b, s32 i, s32 cur);
void func_02036854(void *p);
void func_0201e7cc(s32 a, void *b);
void func_020367b4(s32 a, s32 b);
void func_02044718(void);

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
    t = func_020372d0(func_0204dbb8, func_0204dccc, 7, 0x6100, 3, 4);
    if (t == (UnkTask_0204dce8 *)func_02006abc(0))
        return;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, t->work, 4);
    }
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1000;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1000;
    *(vu16 *)0x5000000 = 0;
    *(vu16 *)0x5000400 = 0;
    func_02043c7c(0x42, 0x100);
    total = 0;
    lang = data_02072cb8;
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
            func_0204da44(tbl.s[LangIdx_0204dce8(lang)][i], x, 0x108, 0x68, i, cur);
            cur += wt.w[tbl.s[LangIdx_0204dce8(lang)][i]];
        }
    }
    func_02036854(data_0207a1d8);
    func_0201e7cc(0xb, data_02250458);
    func_020367b4(0, 0x14);
    func_02044718();
}
