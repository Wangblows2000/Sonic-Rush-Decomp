#include <nitro/types.h>

typedef struct {
    u32 unk0;
    void *unk4;
    void *unk8;
    const char *unkC;
    const char *unk10;
    u32 unk14;
    u8 unk18;
} UnkInfo_020181dc;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
BOOL func_02018128(const char *path, u8 *dst, BOOL big);
u32 func_0201807c(const void *s);

void func_020181dc(u8 *obj, UnkInfo_020181dc *info, const void *name)
{
    BOOL fail1;
    BOOL fail2;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, obj, 0x4c0);
    }
    obj[0x4b2] = 0;
    fail1 = func_02018128(info->unkC, obj, TRUE) == FALSE;
    fail2 = func_02018128(info->unk10, obj, FALSE) == FALSE;
    if (fail1 | fail2) {
        obj[0x4b2] = 1;
        {
            vu32 z = 0;
            MIi_CpuClearFast(z, obj, 0x220);
        }
    }
    *(u32 *)(obj + 0x4b8) = info->unk14;
    if (name != NULL) {
        MIi_CpuCopy16(name, obj + 0x220, 0x16);
    }
    obj[0x236] = info->unk18;
    MIi_CpuCopy16(info->unk4, obj + 0x238, (u16)(func_0201807c(info->unk4) << 1));
    MIi_CpuCopy16(info->unk8, obj + 0x298, 0xc0);
    obj[0x358] = 1;
    *(u16 *)(obj + 0x35a) = 1;
    *(u16 *)(obj + 0x4b0) = 1;
}
