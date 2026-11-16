#include <nitro/types.h>

typedef struct {
    u16 data[15];
} UnkEntry_02040f40;

typedef struct {
    void (*fn)(void);
    u32 unk4;
    s32 unk8;
    s32 unkc;
    u16 unk10;
    u8 pad12[2];
    u16 unk14;
    u8 pad16;
    u8 unk17;
    u16 unk18;
    u16 unk1a;
    u8 pad1c[0x1af4 - 0x1c];
    u8 unk1af4[0x1c];
    u32 unk1b10;
    UnkEntry_02040f40 entries[15];
    u16 cnt[15];
} UnkWork_02040f40;

u16 func_0203b078(s32 a);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
UnkEntry_02040f40 *func_0203af64(u16 i);
s32 func_0203b08c(void);
void func_0203bb30(void *p, u16 a);
void func_0203b6a4(void);
BOOL func_0203b634(void);
void func_0203b54c(void);
void func_0203b52c(void);
void func_02040f34(void);

void func_02040f40(UnkWork_02040f40 *w) {
    u16 prev;
    u16 i;
    UnkEntry_02040f40 *p;
    vu16 zero;

    prev = w->unk14;
    w->unk14 = func_0203b078(5) | (func_0203b078(4) | (func_0203b078(2) | func_0203b078(3))) | 1;
    zero = 0;
    MIi_CpuClear16(zero, w->entries, sizeof(w->entries));
    for (i = 1; i <= 15; i++) {
        p = func_0203af64(i);
        if (p != NULL) {
            if (w->cnt[i - 1] < 4)
                w->cnt[i - 1]++;
            else
                w->entries[i - 1] = *p;
        } else {
            w->cnt[i - 1] = 0;
        }
    }
    w->unk8 = 1;
    switch (func_0203b08c()) {
    case 1:
        w->unk1a = 0;
        func_0203bb30(w->unk1af4, w->unk10);
        return;
    case 2:
        if (w->unk14 & ~1) {
            w->unk8 = 2;
            if (w->unk17 != 0)
                func_0203b6a4();
        }
        w->unk18 = 0;
        w->unk1a = 0;
        return;
    case 3:
        w->unk8 = 3;
        if (w->unk14 == 1) {
            w->unk1a = 0xf0;
            w->unk8 = 7;
            w->unkc = 2;
            return;
        }
        if ((w->unk1b10 & 1) && prev != w->unk14) {
            w->unk1a = 0xf0;
            w->unk8 = 7;
            w->unkc = 2;
            return;
        }
        if (func_0203b634())
            func_0203b54c();
        w->unk1a = 0;
        return;
    case 4:
        w->unk1a = 0;
        return;
    case 5:
        w->unk8 = 6;
        w->unk1a = 0;
        w->fn = func_02040f34;
        return;
    case 7:
        w->unk8 = 7;
        w->unkc = 3;
        func_0203b52c();
        return;
    }
}
