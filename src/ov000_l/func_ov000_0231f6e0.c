#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} VecFx32;

typedef struct {
    u8 pad0[0x20c];
    u32 unk20C;
    u8 pad210[0x9c4 - 0x210];
    VecFx32 ofs;
} UnkStruct_0231f6e0;

extern UnkStruct_0231f6e0 *data_ov000_02369b84;
extern VecFx32 data_ov000_0236a480;

void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab);
void VEC_Add(const VecFx32 *a, const VecFx32 *b, VecFx32 *ab);
s32 func_02044690(void);
s32 func_02044670(void);

void func_ov000_0231f6e0(void) {
    UnkStruct_0231f6e0 *w = data_ov000_02369b84;

    if (w->unk20C & 0x10) {
        if (w->unk20C & 0x20)
            VEC_Subtract(&data_ov000_0236a480, &w->ofs, &data_ov000_0236a480);
        w->unk20C |= 0x20;
        w->ofs.x = func_02044690() << 10;
        w->ofs.y = func_02044670() << 10;
        VEC_Add(&data_ov000_0236a480, &w->ofs, &data_ov000_0236a480);
        return;
    }
    if (w->unk20C & 0x20) {
        VEC_Subtract(&data_ov000_0236a480, &w->ofs, &data_ov000_0236a480);
        w->unk20C &= ~0x20;
    }
}
