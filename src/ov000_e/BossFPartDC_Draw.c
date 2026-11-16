#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    UnkStruct_Vec unk24;
    UnkStruct_Vec unk30;
    UnkStruct_Vec unk3C;
    UnkStruct_Vec unk48;
} UnkStruct_Mdl;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x220 - 0x210];
    s32 unk220;
    s32 unk224;
    s32 unk228;
    u8 pad22C[0x238 - 0x22C];
    void *unk238;
    UnkStruct_Mdl unk23C;
} UnkStruct_022f78bc;

extern UnkStruct_Task *gTaskCurrent;
extern UnkStruct_Vec data_ov000_0236a45c;

s32 func_02002278(UnkStruct_Vec *a, UnkStruct_Vec *b);
void VEC_Subtract(UnkStruct_Vec *a, UnkStruct_Vec *b, UnkStruct_Vec *c);
void func_020024b4(UnkStruct_Vec *a, UnkStruct_Vec *b);
void func_0200265c(UnkStruct_Vec *a, UnkStruct_Vec *b, UnkStruct_Vec *c);
u8 *func_02025ecc(void *p);
void func_02025dac(void *m, s32 a, s32 b);
void Model_Draw(void *m);

static inline s32 Mul64(s64 a, s64 b) {
    return (s32)((a * b + 0x800) >> 12);
}

void BossFPartDC_Draw(void) {
    UnkStruct_022f78bc *w = gTaskCurrent->work;
    UnkStruct_Mdl *m = &w->unk23C;
    UnkStruct_Vec v;
    u8 *r;
    UnkStruct_Vec *p;

    if (w->base.unk5C & 0x20)
        return;
    v.x = w->unk220 << 4;
    v.y = -w->unk224 << 4;
    v.z = w->unk228 << 4;
    m->unk48.x = w->base.unk10 << 4;
    m->unk48.y = -(w->base.unk14 << 4);
    m->unk48.z = w->base.unk18 << 4;
    m->unk20 = func_02002278(&v, &m->unk48);
    m->unk20 = Mul64(m->unk20, 0x1199);
    p = &m->unk3C;
    VEC_Subtract(&v, &m->unk48, p);
    func_020024b4(p, p);
    p = &m->unk24;
    VEC_Subtract(&data_ov000_0236a45c, &m->unk48, &m->unk30);
    func_0200265c(&m->unk30, &m->unk3C, p);
    func_020024b4(p, p);
    p = &m->unk30;
    func_0200265c(&m->unk3C, &m->unk24, p);
    func_020024b4(p, p);
    if (w->unk20C & 8) {
        r = func_02025ecc(w->unk238);
        func_02025dac(r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc), 0, 0x1f0000);
    }
    Model_Draw(m);
    r = func_02025ecc(w->unk238);
    func_02025dac(r + *(u32 *)(r + *(u16 *)(r + 0xe) + 0xc), 1, 0x1f0000);
}
