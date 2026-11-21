#include <nitro/types.h>

typedef struct {
    u32 offset;
    u32 length;
} UnkRomRegion_0200bf48;

extern u32 data_0207efc0;
extern u32 data_0207efbc;
extern UnkRomRegion_0200bf48 data_0207efc4;
extern UnkRomRegion_0200bf48 data_0207efcc;
extern u8 data_0207efd4[];
extern char data_02077980[];

s32 OS_GetLockID(void);
void CARD_Init(void);
void FS_InitArchive(void *arc);
BOOL FS_RegisterArchiveName(void *arc, const char *name, u32 len);
void FS_SetArchiveProc(void *arc, void *proc, u32 flags);
BOOL FS_LoadArchive(void *arc, u32 base, u32 fat, u32 fatSize, u32 fnt, u32 fntSize, void *read, void *write);
void FSi_EmptyArchiveProc(void);
void FSi_ReadDummyCallback(void);
void FSi_WriteDummyCallback(void);
void FSi_RomArchiveProc(void);
void FSi_ReadRomCallback(void);

static inline const UnkRomRegion_0200bf48 *func_0200bf48_FNT(void)
{
    return (const UnkRomRegion_0200bf48 *)0x027ffe40;
}

static inline const UnkRomRegion_0200bf48 *func_0200bf48_FAT(void)
{
    return (const UnkRomRegion_0200bf48 *)0x027ffe48;
}

void FSi_InitRom(u32 default_dma_no)
{
    data_0207efc0 = default_dma_no;
    data_0207efbc = OS_GetLockID();
    data_0207efc4.offset = 0;
    data_0207efc4.length = 0;
    data_0207efcc.offset = 0;
    data_0207efcc.length = 0;
    CARD_Init();
    FS_InitArchive(data_0207efd4);
    FS_RegisterArchiveName(data_0207efd4, data_02077980, 3);
    if (*(u16 *)0x027ffc40 == 2) {
        data_0207efc4.offset = (u32)~0;
        data_0207efc4.length = 0;
        data_0207efcc.offset = (u32)~0;
        data_0207efcc.length = 0;
        FS_SetArchiveProc(data_0207efd4, FSi_EmptyArchiveProc, (u32)~0);
        (void)FS_LoadArchive(data_0207efd4, 0, 0, 0, 0, 0, FSi_ReadDummyCallback, FSi_WriteDummyCallback);
    } else {
        const UnkRomRegion_0200bf48 *const fnt = func_0200bf48_FNT();
        const UnkRomRegion_0200bf48 *const fat = func_0200bf48_FAT();
        FS_SetArchiveProc(data_0207efd4, FSi_RomArchiveProc, 0x602);
        if ((fnt->offset == 0xffffffff) || (fnt->offset == 0) || (fat->offset == 0xffffffff) || (fat->offset == 0)) {
        } else {
            (void)FS_LoadArchive(data_0207efd4, 0, fat->offset, fat->length, fnt->offset, fnt->length, FSi_ReadRomCallback, FSi_WriteDummyCallback);
        }
    }
}
