#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[8];
    u16 unk10;
    u16 unk12;
    u8 pad14[6];
    u16 unk1a;
    u8 pad1c[0x1aee - 0x1c];
    u16 unk1aee;
    u16 unk1af0;
    u16 unk1af2;
} UnkWork_02041d8c;

extern u32 data_02087cb8;
extern u8 data_022c43a0[];

u32 func_0203e934(void);
void func_0203e9a8(void *p, u32 size);
void func_0203e970(u16 a);
void func_02035018(u16 a);
void func_0203e954(u16 a, u16 b);
void func_0203e10c(s32 state, u16 a, u16 b);
void func_02041c14(void);

void func_02041d8c(UnkWork_02041d8c *w) {
    s32 state;

    if (func_0203e934() != 1)
        return;
    func_0203e9a8(data_022c43a0, 0x70);
    switch (w->mode) {
    case 1:
        state = 4;
        data_02087cb8 |= 0x100;
        data_02087cb8 &= ~0x200;
        func_0203e970(w->unk1aee);
        func_02035018(w->unk1aee);
        break;
    case 2:
        state = 6;
        func_0203e954(w->unk1af0, w->unk1af2);
        break;
    }
    func_0203e10c(state, w->unk12, w->unk10);
    w->unk1a = 0;
    w->fn = func_02041c14;
}
