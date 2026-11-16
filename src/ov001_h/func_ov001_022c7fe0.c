#include <nitro/types.h>

typedef struct UnkStruct_022c7fe0 {
    u32 unk0;
    u8 pad4[0x10 - 4];
    void *res[0x13];
    u8 bgs[0x13][0x40];
    void *hdr[2];
} UnkStruct_022c7fe0;

extern u8 data_02072cb8;
extern char **data_ov001_02318ae8[];
extern char **data_ov001_02318aa8[];
extern char **data_ov001_02318ac8[];
extern u8 data_ov001_02319f78[];
extern s32 data_ov001_02315ec4[];
extern void *data_02250458;

void *func_02033fbc(void *p, s32 v);
void func_02036854(void *a);
void func_0201e7cc(s32 a, void *b);

static inline u32 GetLang(void) {
    switch (data_02072cb8) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return data_02072cb8;
    default:
        return 1;
    }
}

void func_ov001_022c7fe0(UnkStruct_022c7fe0 *w) {
    char **b;
    s32 i;
    char **a;

    if (w->unk0 == 0)
        a = data_ov001_02318ae8[GetLang()];
    else
        a = data_ov001_02318aa8[GetLang()];
    b = data_ov001_02318ac8[GetLang()];
    for (i = 0; i < 0x13; i++)
        w->res[i] = func_02033fbc(a[i], -1);
    for (i = 0; i < 2; i++)
        w->hdr[i] = func_02033fbc(b[i], -1);
    func_02036854(data_ov001_02319f78);
    func_0201e7cc(data_ov001_02315ec4[w->unk0], data_02250458);
}
