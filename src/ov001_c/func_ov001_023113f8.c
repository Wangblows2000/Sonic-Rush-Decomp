#include <nitro/types.h>

typedef struct UnkStruct_023113f8 {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[0x20 - 0x14];
    s32 x;
    s32 y;
    s32 z;
    u8 pad2C[0xac - 0x2c];
    u8 *unkAC;
    u8 padB0[0x128 - 0xb0];
    s32 unk128;
    u8 pad12C[0x150 - 0x12c];
    s16 unk150;
    s16 unk152;
    s32 unk154;
} UnkStruct_023113f8;

extern u32 gRandSeed;

void func_0201c000(s32 a, s32 b, s32 c);
void func_ov001_0231599c(s32 v);
void func_ov001_023122d8(UnkStruct_023113f8 *p, s32 v);
void func_ov001_0230d6f4(s32 v);
void func_ov001_023098b4(void);
void *func_ov001_0230c090(void);
void func_ov001_0230f4c8(void *g, s32 x, s32 y, s32 z);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov001_023113f8(UnkStruct_023113f8 *p) {
    p->unk12 = 0;
    if (p->unkAC[0x10c] & 2) {
        func_0201c000(0x10, 0xf, 0x5a);
        if (p->unk152 >= 0) {
            p->unk150 = p->unk152;
            p->unk128 = 0;
            func_ov001_023122d8(p, 0xf);
            func_ov001_023098b4();
        } else {
            func_ov001_0231599c(0);
            func_ov001_023122d8(p, 0xe);
            p->unk128 = 0;
            func_ov001_0230d6f4(0x800);
        }
        return;
    }
    if (p->unk154 <= 0)
        return;
    func_ov001_0230f4c8(func_ov001_0230c090(), p->x + (Rand() % 100 - 0x32) * 0x1000, p->y + (Rand() % 100 - 0x32) * 0x1000, p->z + (Rand() % 100 - 0x32) * 0x1000);
    p->unk154--;
}
