#include <nitro/types.h>

typedef struct UnkStruct_022c7ac8 UnkStruct_022c7ac8;

struct UnkStruct_022c7ac8 {
    u8 pad0[4];
    s32 unk4;
    void (*unk8)(UnkStruct_022c7ac8 *w);
    s32 unkC;
    u8 pad10[0x5b4 - 0x10];
    u8 spr5B4[0x644 - 0x5b4];
    u8 spr644[0x68c - 0x644];
    s32 unk68C;
    s32 unk690;
    s32 unk694;
    s32 unk698;
    s32 unk69C;
};

typedef struct UnkTask_022c7ac8 {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[0x10 - 0xc];
    UnkStruct_022c7ac8 *unk10;
} UnkTask_022c7ac8;

typedef struct UnkTouch_022c7ac8 {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padC[0x10 - 0xc];
    u32 flags;
} UnkTouch_022c7ac8;

typedef struct UnkPad_022c7ac8 {
    u8 pad0[4];
    u16 trg;
} UnkPad_022c7ac8;

typedef struct UnkBright_022c7ac8 {
    u8 pad0[0x48];
    s16 brightness;
} UnkBright_022c7ac8;

extern UnkTask_022c7ac8 *data_022b4574;
extern UnkTouch_022c7ac8 data_022b65b4;
extern UnkPad_022c7ac8 data_022503e0;
extern u8 data_02250464[];
extern UnkBright_022c7ac8 data_0208922c;
extern UnkBright_022c7ac8 data_02087d04;

void func_0203344c(void *s, s32 a, s32 b);
void func_02032d2c(void *s);
s32 func_ov001_022c678c(void *s, u16 x, u16 y);
void func_0201c070(void *p, s32 a);
void func_02033550(void *s, s32 a);
void func_ov001_022c7a44(UnkStruct_022c7ac8 *w);
void func_ov001_022c6804(UnkStruct_022c7ac8 *w);
void func_ov001_022c7a34(UnkStruct_022c7ac8 *w, s32 a);
void func_ov001_022c8458(void);

void func_ov001_022c7ac8(void) {
    UnkStruct_022c7ac8 *w = data_022b4574->unk10;
    void *s;

    w->unk4++;
    w->unkC++;
    if (w->unk68C == 1) {
        if (w->unk690 != 0 || (w->unk4 & 0x30)) {
            s = w->spr644;
            func_0203344c(s, 0, 0);
            func_02032d2c(s);
        }
        s = w->spr5B4;
        func_0203344c(s, 0, 0);
        func_02032d2c(s);
    }
    if (w->unk4 > 0x3c && w->unk68C == 1 && w->unk690 == 0) {
        s32 r = 0;
        if (data_022b65b4.flags & 0x40)
            r = func_ov001_022c678c(w->spr5B4, data_022b65b4.x, data_022b65b4.y);
        if (r == 1 || (data_022503e0.trg & 8)) {
            w->unk690 = 1;
            w->unk694 = 0x3c;
            func_0201c070(data_02250464, 0x3c);
            func_02033550(w->spr644, 2);
            func_02033550(w->spr5B4, 3);
        }
    }
    func_ov001_022c7a44(w);
    func_ov001_022c6804(w);
    if (w->unk8 != NULL)
        w->unk8(w);
    if (w->unk690 != 0) {
        w->unk694--;
        if (w->unk694 % 3 == 0) {
            data_0208922c.brightness++;
            data_02087d04.brightness++;
        }
        if (data_0208922c.brightness > 16)
            data_0208922c.brightness = 16;
        if (data_02087d04.brightness > 16)
            data_02087d04.brightness = 16;
        if (w->unk694 <= 0 && data_0208922c.brightness == 16 && data_02087d04.brightness == 16) {
            func_ov001_022c7a34(w, 0);
            data_022b4574->unk8 = func_ov001_022c8458;
            return;
        }
    }
    w->unk69C = w->unk698;
}
