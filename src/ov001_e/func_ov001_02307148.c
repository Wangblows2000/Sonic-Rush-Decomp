#include <nitro/types.h>

typedef struct UnkStruct_02307148_p {
    u8 pad0[0x38];
    s32 *unk38;
} UnkStruct_02307148_p;

typedef struct UnkStruct_02307148 {
    u8 pad0[0x16];
    u16 ang;
    s32 y;
    s32 z;
    u8 pad20[0x64 - 0x20];
    u16 unk64;
    u8 pad66[0x78 - 0x66];
    s32 *unk78;
    u8 unk7C[4];
} UnkStruct_02307148;

UnkStruct_02307148 *func_ov001_02306db0(UnkStruct_02307148_p *parent, s32 type);
s32 func_ov001_02310144(UnkStruct_02307148 *n);
void func_ov001_0230d28c(void *m, s32 mode, s32 flag, s32 force, u32 ang, s32 y, s32 z);

UnkStruct_02307148 *func_ov001_02307148(UnkStruct_02307148_p *parent, s32 type, u16 ang, s32 y, s32 z) {
    UnkStruct_02307148 *n = func_ov001_02306db0(parent, type);
    s32 flag;

    if (n == NULL)
        return NULL;
    n->ang = ang;
    n->y = y;
    n->z = z;
    if (z >= 0x12c000) {
        n->unk78 = parent->unk38;
        n->z -= 0x12c000;
    }
    if (parent != NULL) {
        if (n->unk64 & 0x2000)
            flag = 1;
        else
            flag = 0;
        func_ov001_0230d28c(n->unk7C, *n->unk78, flag, func_ov001_02310144(n), n->ang, n->y, n->z);
    }
    return n;
}
