#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 top;     /* 0x20 */
    u32 bottom;  /* 0x24 */
    u32 pos;     /* 0x28 */
} UnkFSFile_0200b930;

BOOL FS_SeekFile(UnkFSFile_0200b930 *p_file, s32 offset, u32 origin)
{
    switch (origin) {
    case 0:
        offset += p_file->top;
        break;
    case 1:
        offset += p_file->pos;
        break;
    case 2:
        offset += p_file->bottom;
        break;
    default:
        return FALSE;
    }
    if (offset < (s32)p_file->top) {
        offset = (s32)p_file->top;
    }
    if (offset > (s32)p_file->bottom) {
        offset = (s32)p_file->bottom;
    }
    p_file->pos = (u32)offset;
    return TRUE;
}
