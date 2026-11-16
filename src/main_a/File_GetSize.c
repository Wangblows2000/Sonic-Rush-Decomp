#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; u32 top; u32 bottom; u8 unk28[0x20]; } UnkFile_02033ddc;
void func_0200be70(UnkFile_02033ddc *f);
BOOL func_0200bb20(UnkFile_02033ddc *f, void *path);
BOOL func_0200bad8(UnkFile_02033ddc *f);

s32 File_GetSize(void *path) {
    UnkFile_02033ddc f;
    s32 len;
    func_0200be70(&f);
    if (!func_0200bb20(&f, path)) return -1;
    len = f.bottom - f.top;
    if (!func_0200bad8(&f)) len = -1;
    return len;
}
