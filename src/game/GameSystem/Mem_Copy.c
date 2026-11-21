#include <nitro/types.h>

void MIi_CpuCopyFast(const void *src, void *dst, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);

void Mem_Copy(const u8 *src, u8 *dst, u32 size) {
    u32 n;
    if ((((u32)src | (u32)dst) & 3) == 0) {
        n = size & ~0x1f;
        if (n != 0) {
            MIi_CpuCopyFast(src, dst, n);
            src += n;
            dst += n;
            size &= 0x1f;
        }
        n = size & ~3;
        if (n != 0) {
            MIi_CpuCopy32(src, dst, n);
            src += n;
            dst += n;
            size &= 3;
        }
    }
    if ((((u32)src | (u32)dst) & 1) == 0) {
        n = size & ~1;
        if (n != 0) {
            MIi_CpuCopy16(src, dst, n);
            src += n;
            dst += n;
            size &= 1;
        }
    }
    if (size != 0) {
        MI_CpuCopy8(src, dst, size);
    }
}
