#include <nitro/types.h>

typedef struct {
    u32 top;
    u32 bottom;
} UnkFSFAT_0200a35c;

typedef struct {
    u8 pad0[0x20];
    u32 fat;      /* 0x20 */
    u32 fatSize;  /* 0x24 */
} UnkFSArchive_0200a35c;

typedef struct {
    void *arc;
    u32 pos;
} UnkFSReadParam_0200a35c;

typedef struct {
    u8 pad0[8];
    UnkFSArchive_0200a35c *arc;  /* 0x08 */
    u8 padC[0x20];
    u32 arg0;    /* 0x2c */
    u32 arg1;    /* 0x30 */
    u32 arg2;    /* 0x34 */
} UnkFSFile_0200a35c;

void func_0200ac64(UnkFSReadParam_0200a35c *p, void *dst, u32 len);
s32 func_0200ad14(UnkFSFile_0200a35c *f, u32 command);

s32 func_0200a35c(UnkFSFile_0200a35c *p_file)
{
    UnkFSArchive_0200a35c *const p_arc = p_file->arc;
    const u32 index = p_file->arg1;
    {
        u32 pos = index * sizeof(UnkFSFAT_0200a35c);
        if (pos >= p_arc->fatSize) {
            return 1;
        }
        {
            UnkFSFAT_0200a35c fat;
            UnkFSReadParam_0200a35c param;
            param.arc = p_arc;
            param.pos = p_arc->fat + pos;
            func_0200ac64(&param, &fat, sizeof(fat));
            p_file->arg0 = fat.top;
            p_file->arg1 = fat.bottom;
            p_file->arg2 = index;
            return func_0200ad14(p_file, 7);
        }
    }
}
