#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 unk4[0x32];
    u16 unk36;
    u8 pad38[200 - 0x38];
} UnkEntry_020415a8;

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[0x10];
    u16 cnt;
    u16 unk1a;
    u8 pad1c[0xc];
    UnkEntry_020415a8 entries[32];
    u8 pad1928[2];
    u16 idx;
    u8 pad192c[0x1aee - 0x192c];
    u16 unk1aee;
    u16 unk1af0;
    u16 unk1af2;
} UnkWork_020415a8;

extern u32 data_02087cb8;

u32 func_0203e934(void);
void func_0203e970(u16 a);
void func_02035018(u16 a);
void func_0203e954(u16 a, u16 b);
void func_0203de90(void (*fn)(void));
void func_0203f9a4(void (*fn)(void), s32 a, void *p, u16 b);
void func_02042340(void);
void func_02042388(void);
void func_02041484(void);

void func_020415a8(UnkWork_020415a8 *w) {
    UnkEntry_020415a8 *e;
    s32 state;

    if (func_0203e934() != 1)
        return;
    w->unk1a = 0;
    if (w->cnt >= 10) {
        e = &w->entries[w->idx];
        switch (w->mode) {
        case 1:
            state = 5;
            data_02087cb8 |= 0x100;
            data_02087cb8 &= ~0x200;
            func_0203e970(w->unk1aee);
            func_02035018(w->unk1aee);
            break;
        case 2:
            state = 7;
            func_0203e954(w->unk1af0, w->unk1af2);
            func_0203de90(func_02042340);
            break;
        }
        func_0203f9a4(func_02042388, state, e->unk4, e->unk36);
        w->fn = func_02041484;
    } else {
        w->cnt++;
    }
}
