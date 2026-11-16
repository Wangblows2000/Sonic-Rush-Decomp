#include <nitro/types.h>

typedef struct UnkStruct_02313cfc {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x34 - 2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x1DC - 0x5E];
    void *unk1DC;
    u8 pad1E0[0x204 - 0x1E0];
    struct UnkStruct_02313cfc *unk204;
    u8 pad208[0x3E8 - 0x208];
    struct UnkStruct_02313cfc *unk3E8;
} UnkStruct_02313cfc;

typedef struct {
    u8 pad0[0xDA];
    s16 unkDA;
    u8 padDC[0x122 - 0xDC];
    s16 unk122;
} UnkStruct_02313cfc_Eff;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02313cfc_4560;

extern UnkStruct_02313cfc_4560 data_022c4560;
extern void *data_ov000_0236ac60[];
extern u32 data_0208fe70;

void func_02068204(void *a, UnkStruct_02313cfc *w, void *b);
void func_020693f0(s16 *x, s16 *y, u8 a);
UnkStruct_02313cfc_Eff *func_ov000_0232e48c(UnkStruct_02313cfc *w, s16 x, s16 y);
void func_ov000_02313bb0(UnkStruct_02313cfc *w);
s16 func_02068df8(s16 v, s32 a, s32 b);

static inline u16 UnkRand_02313cfc(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_02313cfc(UnkStruct_02313cfc *w) {
    if (w->unk204 != NULL) {
        if (w->unk204->unk3E8 == w) {
            w->unk1 = w->unk204->unk1;
        } else {
            w->unk204 = NULL;
        }
    } else {
        func_02068204(w->unk1DC, w, data_ov000_0236ac60[0]);
    }
    if (!(data_022c4560.unk20 & 3)) {
        s16 x;
        s16 y;
        UnkStruct_02313cfc_Eff *e;
        x = 0x33 - (UnkRand_02313cfc() & 6);
        y = 3 - (UnkRand_02313cfc() & 6);
        func_020693f0(&x, &y, w->unk1);
        e = func_ov000_0232e48c(w, x, y);
        e->unkDA = 0x15;
        e->unk122 = 0x15;
    }
    if ((w->unk58 & 0xf) || w->unk34 == 0) {
        func_ov000_02313bb0(w);
    }
    if (w->unk5C & 1) {
        w->unk34 = func_02068df8(w->unk34, 0x10, 0xc00);
    } else {
        w->unk34 = func_02068df8(w->unk34, -0x10, 0xc00);
    }
}
