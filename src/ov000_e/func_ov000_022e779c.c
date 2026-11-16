#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x42C - 0x20C];
    u16 unk42C;
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void *unk440;
} UnkStruct_022e779c;

extern UnkStruct_022c4560 data_022c4560;
extern u16 data_ov000_02363b94[][4];

void func_ov000_022e8c90(UnkStruct_022e779c *w);
void func_ov000_022ee198(UnkStruct_022e779c *w, s32 st);
void func_ov000_022e773c(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

void func_ov000_022e779c(UnkStruct_022e779c *w) {
    u16 t;

    t = data_ov000_02363b94[GetIdxB()][w->unk42E];
    if (t >= (Rand() & 0xf)) {
        w->unk42C = w->unk42E;
        func_ov000_022e8c90(w);
        return;
    }
    func_ov000_022ee198(w, 0x13);
    w->base.unk58 |= 0x10;
    w->base.unk58 |= 0x80;
    w->base.unk30 = -0x280;
    w->unk440 = func_ov000_022e773c;
}
