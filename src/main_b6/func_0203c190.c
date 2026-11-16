#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
} UnkCtx_0203c190;

typedef struct {
    u8 pad0[0x34];
    void *unk34;
} UnkFile_0203c190;

extern UnkCtx_0203c190 data_022b8be0;

void func_0203d268(UnkFile_0203c190 *f);

s32 func_0203c190(UnkFile_0203c190 *f, s32 type) {
    switch (type) {
    case 0:
        if (data_022b8be0.unkc != 2)
            return 5;
        if (f->unk34 == NULL)
            return 0;
        func_0203d268(f);
        return 6;
    case 1:
        return 4;
    default:
        return 8;
    }
}
