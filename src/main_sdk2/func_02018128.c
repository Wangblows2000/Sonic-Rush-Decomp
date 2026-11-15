#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 top;
    u32 bottom;
    u8 pad28[0x20];
} UnkFSFile_02018128;

void func_0200be70(UnkFSFile_02018128 *f);
BOOL func_0200bb20(UnkFSFile_02018128 *f, const char *path);
s32 func_0200b99c(UnkFSFile_02018128 *f, void *dst, u32 size);
void func_0200bad8(UnkFSFile_02018128 *f);

BOOL func_02018128(const char *path, u8 *dst, BOOL big)
{
    UnkFSFile_02018128 file;
    u32 size = big ? 0x200 : 0x20;
    if (big) {
        dst += 0x20;
    }
    if (path == NULL) {
        return FALSE;
    }
    func_0200be70(&file);
    if (!func_0200bb20(&file, path)) {
        return FALSE;
    }
    if (size != file.bottom - file.top) {
        func_0200bad8(&file);
        return FALSE;
    }
    func_0200b99c(&file, dst, size);
    func_0200bad8(&file);
    return TRUE;
}
