#include <nitro/types.h>

typedef struct {
    u32 name[4];
} UnkResName_02037f94;

typedef struct {
    u8 unk0[0x2c];
    u32 unk2C;
    u8 unk30[4];
    u16 dictOfs;
} UnkTex_02037f94;

u32 strlen(const char *s);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void *NNS_G3dGetResDataByName(void *dict, const UnkResName_02037f94 *name);

u32 Asset3D_GetPaletteKeyByName(UnkTex_02037f94 *tex, const char *name)
{
    u32 len = strlen(name);
    UnkResName_02037f94 rn;
    u16 *e;
    {
        vu32 zero = 0;
        if (len > 16)
            len = 16;
        MIi_CpuClear32(zero, &rn, sizeof(rn));
    }
    MI_CpuCopy8(name, &rn, len);
    e = NNS_G3dGetResDataByName((u8 *)tex + tex->dictOfs, &rn);
    return ((tex->unk2C & 0xffff) + *e) << 3;
}
