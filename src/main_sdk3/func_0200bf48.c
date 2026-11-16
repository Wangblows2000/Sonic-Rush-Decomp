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
void func_0200fd28(void);
void func_0200b2cc(void *arc);
BOOL func_0200b20c(void *arc, const char *name, u32 len);
void func_0200aee8(void *arc, void *proc, u32 flags);
BOOL func_0200b0fc(void *arc, u32 base, u32 fat, u32 fatSize, u32 fnt, u32 fntSize, void *read, void *write);
void func_0200c0e8(void);
void func_0200c0f0(void);
void func_0200c188(void);
void func_0200c0f8(void);
void func_0200c190(void);

static inline const UnkRomRegion_0200bf48 *func_0200bf48_FNT(void)
{
    return (const UnkRomRegion_0200bf48 *)0x027ffe40;
}

static inline const UnkRomRegion_0200bf48 *func_0200bf48_FAT(void)
{
    return (const UnkRomRegion_0200bf48 *)0x027ffe48;
}

void func_0200bf48(u32 default_dma_no)
{
    data_0207efc0 = default_dma_no;
    data_0207efbc = OS_GetLockID();
    data_0207efc4.offset = 0;
    data_0207efc4.length = 0;
    data_0207efcc.offset = 0;
    data_0207efcc.length = 0;
    func_0200fd28();
    func_0200b2cc(data_0207efd4);
    func_0200b20c(data_0207efd4, data_02077980, 3);
    if (*(u16 *)0x027ffc40 == 2) {
        data_0207efc4.offset = (u32)~0;
        data_0207efc4.length = 0;
        data_0207efcc.offset = (u32)~0;
        data_0207efcc.length = 0;
        func_0200aee8(data_0207efd4, func_0200c0e8, (u32)~0);
        (void)func_0200b0fc(data_0207efd4, 0, 0, 0, 0, 0, func_0200c0f0, func_0200c188);
    } else {
        const UnkRomRegion_0200bf48 *const fnt = func_0200bf48_FNT();
        const UnkRomRegion_0200bf48 *const fat = func_0200bf48_FAT();
        func_0200aee8(data_0207efd4, func_0200c0f8, 0x602);
        if ((fnt->offset == 0xffffffff) || (fnt->offset == 0) || (fat->offset == 0xffffffff) || (fat->offset == 0)) {
        } else {
            (void)func_0200b0fc(data_0207efd4, 0, fat->offset, fat->length, fnt->offset, fnt->length, func_0200c190, func_0200c188);
        }
    }
}
