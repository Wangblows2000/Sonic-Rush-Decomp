#include <nitro/types.h>

typedef struct {
    u8 pad0[0x22];
    u16 delay[12];
    u16 rate[12];
} UnkPad_022e8a24;

typedef struct {
    u8 pad0[8];
    u32 flags;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x1cc4];
    s32 unk1cc4;
} UnkWork_022e8a24;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
} UnkRes_022e8a24;

extern UnkPad_022e8a24 data_022503e0;
extern s32 data_022c4560;
extern UnkStruct_022c4680 data_022c4680;
extern s32 data_02087ccc;
extern u32 data_02087cb8;

void func_0204ec40(void);
UnkWork_022e8a24 *func_ov001_022e8410(void);
void func_ov001_022e89b4(void);
void func_ov001_022e8784(UnkWork_022e8a24 *w);
void func_ov001_022e870c(UnkWork_022e8a24 *w);
void func_020522f4(void);
s32 func_ov001_022e2ad8(void);
s32 func_ov001_022e33c0(s32 a, s32 b);
UnkRes_022e8a24 *func_ov001_022e8090(UnkWork_022e8a24 *w, s32 a, s32 b);
void func_02063f50(void);
void func_02037d60(void);
void func_02052168(s32 a);

#pragma thumb on
void func_ov001_022e8a24(s32 a, s32 b) {
    UnkWork_022e8a24 *w;
    UnkRes_022e8a24 *r;
    s32 i;
    BOOL flag;

    func_0204ec40();
    w = func_ov001_022e8410();
    func_ov001_022e89b4();
    func_ov001_022e8784(w);
    func_ov001_022e870c(w);
    func_020522f4();
    w->unk1cc4 = func_ov001_022e33c0(func_ov001_022e2ad8(), a);
    r = func_ov001_022e8090(w, a, b);
    func_02063f50();
    for (i = 0; i < 12; i++) {
        data_022503e0.delay[i] = 0x14;
        data_022503e0.rate[i] = 1;
    }
    func_02037d60();
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1900;
    flag = FALSE;
    if (data_022c4560 == 0) {
        if (r->unk4 >= 0x1f) flag = TRUE;
    } else {
        if (r->unk8 >= 0x1e) flag = TRUE;
    }
    if (data_022c4680.flags & 0x1000) flag = FALSE;
    if (flag) {
        func_02052168(5);
    } else {
        func_02052168(4);
    }
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
}
#pragma thumb reset
