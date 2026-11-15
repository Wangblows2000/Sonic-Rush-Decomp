#include <nitro/types.h>

typedef struct {
    u32 type;     /* 0x00 */
    u8 pad4[4];
    u16 bitmap;   /* 0x08 */
    u8 padA[6];
    u32 unk10;    /* 0x10 */
    u8 data[9];   /* 0x14 */
    u8 size;      /* 0x1d */
    u8 pad1e[0x9c - 0x1e];
} UnkMbParam_02015168;

void MI_CpuCopy8(const void *src, void *dest, u32 size);
BOOL func_0201559c(UnkMbParam_02015168 *p);

BOOL func_02015168(u16 bitmap, const void *src, s32 size, u32 d)
{
    UnkMbParam_02015168 p;
    if (size < 0 || size > 9) {
        return FALSE;
    }
    p.bitmap = bitmap;
    p.type = 10;
    p.unk10 = d;
    MI_CpuCopy8(src, p.data, size);
    p.size = size;
    return func_0201559c(&p);
}
