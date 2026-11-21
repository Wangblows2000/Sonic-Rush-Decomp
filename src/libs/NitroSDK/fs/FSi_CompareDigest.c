#include <nitro/types.h>

extern u8 *data_02077984;
extern u32 data_02077988;

void MI_CpuFill8(void *dest, u8 data, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void DGT_Hash2CalcHmac(void *digest, const void *bin, u32 binLen, const u8 *key, s32 keyLen);

BOOL FSi_CompareDigest(const u8 *digest, const void *data, u32 len)
{
    u8 hash[20];
    u8 key[64];
    u32 i;

    MI_CpuFill8(hash, 0, 20);
    MI_CpuCopy8(data_02077984, key, data_02077988);
    DGT_Hash2CalcHmac(hash, data, len, key, data_02077988);
    for (i = 0; i < 20; i += 4) {
        if (*(u32 *)&hash[i] != *(u32 *)&digest[i]) {
            break;
        }
    }
    return i == 20;
}
