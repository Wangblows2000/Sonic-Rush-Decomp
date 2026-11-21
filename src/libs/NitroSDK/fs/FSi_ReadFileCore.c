#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    vu32 stat;     /* 0x0c */
    u8 pad10[0x14];
    u32 bottom;    /* 0x24 */
    u32 pos;       /* 0x28 */
    void *dst;     /* 0x2c */
    u32 lenOrg;    /* 0x30 */
    u32 len;       /* 0x34 */
} UnkFSFile_0200bc88;

BOOL FSi_SendCommand(UnkFSFile_0200bc88 *f, u32 command);
BOOL FS_WaitAsync(UnkFSFile_0200bc88 *f);

s32 FSi_ReadFileCore(UnkFSFile_0200bc88 *p_file, void *dst, s32 len, BOOL async)
{
    const s32 pos = p_file->pos;
    const s32 rest = (s32)(p_file->bottom - pos);
    const u32 dst_len = (u32)len;

    if (len > rest) {
        len = rest;
    }
    if (len < 0) {
        len = 0;
    }
    p_file->dst = dst;
    p_file->lenOrg = dst_len;
    p_file->len = (u32)len;
    if (!async) {
        p_file->stat |= 4;
    }
    (void)FSi_SendCommand(p_file, 0);
    if (!async) {
        if (FS_WaitAsync(p_file)) {
            len = p_file->pos - pos;
        } else {
            len = -1;
        }
    }
    return len;
}
