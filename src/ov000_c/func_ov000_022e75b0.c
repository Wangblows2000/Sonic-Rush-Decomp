#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x440 - 0x20C];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    s32 unk464;
    s32 unk468;
    s32 unk46C;
    s32 unk470;
    s32 unk474;
    s32 unk478;
    u16 unk47C;
} UnkStruct_022e75b0;

extern UnkStruct_StageObj *data_ov000_0236ac60;
extern u8 data_02250464[];

void func_ov000_022ee198(UnkStruct_022e75b0 *w, s32 a);
void func_ov000_022e9b70(UnkStruct_022e75b0 *w, s32 a, s32 b);
void func_0201be40(void *a, s32 b, s32 c);
void func_ov000_022e73e0(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

void func_ov000_022e75b0(UnkStruct_022e75b0 *w) {
    if (!(w->base.unk58 & 1))
        return;
    if (!(data_ov000_0236ac60->unk58 & 1))
        return;
    if ((s16)w->base.unk1F0 > 0)
        return;
    func_ov000_022ee198(w, 0x3c);
    func_ov000_022e9b70(w, 0, 0);
    w->base.unk5C |= 4;
    data_ov000_0236ac60->unk5C &= ~1;
    w->base.unk5C |= 1;
    w->unk464 = data_ov000_0236ac60->unk10 << 4;
    w->unk468 = data_ov000_0236ac60->unk14 << 4;
    w->unk470 = w->base.unk10 << 4;
    w->unk474 = w->base.unk14 << 4;
    w->unk47C = 0;
    w->unk46C = (data_ov000_0236ac60->unk10 >> 8) - 0x40;
    w->unk46C = ABS(w->unk46C);
    w->unk46C >>= 1;
    w->unk46C = CLAMP(w->unk46C, 0x10, 0x28);
    w->unk478 = (w->base.unk10 >> 8) - 0x100;
    w->unk478 = ABS(w->unk478);
    w->unk478 >>= 1;
    w->unk478 = CLAMP(w->unk478, 0x10, 0x28);
    func_0201be40(data_02250464, 0x20, 0xb4);
    w->unk440 = func_ov000_022e73e0;
}
