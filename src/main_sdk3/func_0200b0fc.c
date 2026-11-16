#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    vu32 flag;          /* 0x10 */
    u8 pad14[8];
    u32 base;           /* 0x1c */
    u32 fat;            /* 0x20 */
    u32 fatSize;        /* 0x24 */
    u32 fnt;            /* 0x28 */
    u32 fntSize;        /* 0x2c */
    u32 fatBak;         /* 0x30 */
    u32 fntBak;         /* 0x34 */
    void *loadMem;      /* 0x38 */
    void *readFunc;     /* 0x3c */
    void *writeFunc;    /* 0x40 */
    void *tableFunc;    /* 0x44 */
} UnkFSArchive_0200b0fc;

void func_0200b8a4(void);
void func_0200b878(void);

BOOL func_0200b0fc(UnkFSArchive_0200b0fc *p_arc, u32 base, u32 fat, u32 fat_size, u32 fnt, u32 fnt_size, void *read_func, void *write_func)
{
    p_arc->base = base;
    p_arc->fatSize = fat_size;
    p_arc->fat = p_arc->fatBak = fat;
    p_arc->fntSize = fnt_size;
    p_arc->fnt = p_arc->fntBak = fnt;
    p_arc->readFunc = (read_func != NULL) ? read_func : (void *)func_0200b8a4;
    p_arc->writeFunc = (write_func != NULL) ? write_func : (void *)func_0200b878;
    p_arc->tableFunc = p_arc->readFunc;
    p_arc->loadMem = NULL;
    p_arc->flag |= 2;
    return TRUE;
}
