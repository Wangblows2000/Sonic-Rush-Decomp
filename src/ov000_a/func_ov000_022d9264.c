#include <nitro/types.h>

typedef struct UnkStruct_022d9264_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x3d0 - 0x58];
    u8 unk3D0;
} UnkStruct_022d9264_o;

typedef struct UnkStruct_022d9264 {
    u8 pad[0x214];
    UnkStruct_022d9264_o *unk214;
    UnkStruct_022d9264_o *unk218;
    u8 pad1[0x224 - 0x21c];
    u16 unk224;
} UnkStruct_022d9264;

typedef struct UnkStruct_022d9264_g {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022d9264_g;

typedef struct UnkStruct_022d9264_p {
    u8 pad[0x3ae];
    s16 unk3AE;
} UnkStruct_022d9264_p;

typedef struct UnkStruct_022d9264_c {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022d9264_c;

extern UnkStruct_022d9264_g data_0208922c;
extern UnkStruct_022d9264_p *data_ov000_0236ac60;
extern UnkStruct_022d9264_c data_022c4590;

void func_0204eb04(void);
void func_0204d5f8(void (*fn)(void));

void func_ov000_022d9264(UnkStruct_022d9264 *s) {
    UnkStruct_022d9264_o *a = s->unk214;
    UnkStruct_022d9264_o *b = s->unk218;
    s->unk224++;
    if (!(s->unk224 & 3)) {
        if (data_0208922c.unk48 > 0)
            data_0208922c.unk48--;
    }
    if (!(a->unk3D0 & 2))
        return;
    if (s->unk224 <= 0x12c)
        return;
    a->unk54 |= 4;
    b->unk54 |= 4;
    data_022c4590.unk10 = data_ov000_0236ac60->unk3AE * 1000;
    func_0204d5f8(func_0204eb04);
}
