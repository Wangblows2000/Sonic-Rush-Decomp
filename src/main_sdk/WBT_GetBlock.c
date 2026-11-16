#include <nitro/types.h>

typedef struct { u32 type; u32 pad; u16 a; u8 pad2[6]; u32 e; u32 b; u32 d; u8 buf1[0x40]; u8 buf2[0x40]; } UnkStruct_020151c4;
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void WBT_Unk_0201559c(UnkStruct_020151c4 *p);
void WBT_GetBlock(u16 a, u32 b, const void *c, u32 d, const void *f, u32 e) {
    UnkStruct_020151c4 s;
    s.b = b; s.a = a; s.type = 4; s.e = e; s.d = d;
    MIi_CpuCopy32(c, s.buf1, 0x40);
    MIi_CpuCopy32(f, s.buf2, 0x40);
    WBT_Unk_0201559c(&s);
}
