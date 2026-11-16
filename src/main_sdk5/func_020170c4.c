#include <nitro/types.h>
#include <main_sdk2/mbseg.h>

extern u32 data_02072a18[];

void func_02016f0c(u8 *rom, u32 *type, UnkMbSeg *seg, u32 *cur);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

typedef struct {
    u32 unk0;
    u32 unk4;
    u8 pad8[4];
    UnkMbSeg seg[3];
    u8 unk3c[0x88];
} UnkMbHeader_020170c4;

void func_020170c4(UnkMbHeader_020170c4 *dst, u8 *rom)
{
    u32 cur;
    UnkMbSeg *seg;
    u32 *type;
    s32 i;
    u8 *ext;
    cur = 0x022c0000;
    dst->unk0 = *(u32 *)(rom + 0x24);
    type = data_02072a18;
    dst->unk4 = *(u32 *)(rom + 0x34);
    ext = rom + 0x160;
    seg = dst->seg;
    for (i = 0; i < 3; i++) {
        func_02016f0c(rom, type, seg, &cur);
        seg++;
        type++;
    }
    MI_CpuCopy8(ext, dst->unk3c, 0x88);
}
