#include <nitro/types.h>

typedef struct {
    u8 pad0[0x328];
    s16 unk328;
    u8 pad32A[0x420 - 0x32A];
    u16 unk420;
} UnkStruct_023389f8_Ply;

typedef struct {
    u8 pad0[0x3A];
    s8 unk3A;
    u8 pad3B[0xC8 - 0x3B];
    u32 unkC8;
} UnkStruct_023389f8_Cam;

typedef struct {
    u32 unk0;
    u8 pad4[2];
    s16 unk6;
    s32 unk8;
    void (*unkC)(void);
} UnkStruct_023389f8;

extern UnkStruct_023389f8_Cam data_ov000_0236aad8;
extern UnkStruct_023389f8_Ply *data_ov000_0236ac60[];

void func_ov000_02338800(UnkStruct_023389f8 *w);
void func_ov000_0233887c(void);
void func_ov000_0233894c(void);

void func_ov000_023389f8(UnkStruct_023389f8 *w) {
    UnkStruct_023389f8_Cam *c = &data_ov000_0236aad8;
    UnkStruct_023389f8_Ply *p;
    u16 fl;

    if (c->unkC8 & 0x40000) {
        func_ov000_02338800(w);
        return;
    }
    p = data_ov000_0236ac60[c->unk3A];
    fl = p->unk420;
    if (fl & 1) {
        func_ov000_02338800(w);
        return;
    }
    if ((w->unk0 & 2) && (!(fl & 0x40) || (p->unk328 != 0x21 && p->unk328 != 0x22))) {
        func_ov000_02338800(w);
        return;
    }
    if ((w->unk0 & 4) && (!(fl & 0x80) || (p->unk328 != 3 && p->unk328 != 4))) {
        func_ov000_02338800(w);
        return;
    }
    w->unk6++;
    if (w->unk6 < 0x3c) {
        return;
    }
    w->unk6 = 0;
    if (w->unk0 & 2) {
        data_ov000_0236aad8.unkC8 |= 0x10000;
        w->unk8 = 0;
        w->unkC = func_ov000_0233894c;
    } else {
        data_ov000_0236aad8.unkC8 |= 0x10000;
        w->unk8 = 0;
        w->unkC = func_ov000_0233887c;
    }
}
