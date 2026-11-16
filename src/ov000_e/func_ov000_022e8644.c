#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022e8644 {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x3D8 - 0x3D4];
    u16 unk3D8;
    u8 pad3DA[0x440 - 0x3DA];
    void (*unk440)(struct UnkStruct_022e8644 *);
} UnkStruct_022e8644;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x220 - 0x20C];
    u32 unk220;
} UnkStruct_Boss;

typedef struct {
    u8 pad[0x114];
    s32 unk114;
} UnkStruct_124;

extern UnkStruct_Boss *data_ov000_02369b78;

void func_ov000_02328468(s32 a, s32 x, s32 y, s32 z);
void func_ov000_022eb3c4(UnkStruct_022e8644 *w);
void func_ov000_022e97cc(UnkStruct_022e8644 *w);
void func_ov000_022e98d8(UnkStruct_022e8644 *w);

void func_ov000_022e8644(UnkStruct_022e8644 *w) {
    UnkStruct_Boss *b = data_ov000_02369b78;

    if (w->unk3D0 == 0) {
        if ((b->unk220 & 0x1f) == 0)
            func_ov000_02328468(1, w->base.unk10, w->base.unk14, w->base.unk18 + 8);
    } else {
        if ((b->unk220 & 7) == 0)
            func_ov000_022eb3c4(w);
    }
    func_ov000_022e97cc(w);
    if (w->base.unk2E == 0)
        w->unk440 = func_ov000_022e98d8;
}
