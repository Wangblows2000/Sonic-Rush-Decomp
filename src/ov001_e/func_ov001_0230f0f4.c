#include <nitro/types.h>

typedef struct UnkStruct_0230f0f4 {
    u8 pad0[0x5c];
    s16 unk5C;
    u8 pad5E[2];
    u16 unk60;
    u16 unk62;
    u16 unk64;
    u8 pad66[0x6c - 0x66];
    u32 unk6C;
    u8 pad70[0xc4 - 0x70];
    s32 unkC4;
    u8 padC8[0xd4 - 0xc8];
    struct UnkStruct_0230f0f4 *unkD4;
    s32 unkD8;
} UnkStruct_0230f0f4;

void *func_ov001_0230c0a4(void);
s32 func_ov001_02310248(void *a, UnkStruct_0230f0f4 *p);
UnkStruct_0230f0f4 *func_ov001_023070fc(s32 type, s32 x, s32 y, s32 z);

void func_ov001_0230f0f4(UnkStruct_0230f0f4 *p) {
    UnkStruct_0230f0f4 *r;

    if (p->unkC4 != 0) {
        if (func_ov001_02310248(func_ov001_0230c0a4(), p) == 1) {
            if (!(p->unk62 & 1) && (r = func_ov001_023070fc(5, 0, 0, 0)) != NULL) {
                r->unkD4 = p;
                r->unkD8 = p->unkD8;
                r->unk64 |= 2;
            }
            p->unk62 |= 1;
        } else {
            p->unk62 &= ~1;
        }
    }
    if (p->unk5C <= 0)
        return;
    if (p->unk60 & 0x100) {
        if (p->unk6C & 1)
            p->unk60 |= 4;
        else
            p->unk60 &= ~4;
        p->unk60 |= 8;
    } else {
        p->unk60 &= ~4;
        p->unk60 &= ~8;
    }
}
