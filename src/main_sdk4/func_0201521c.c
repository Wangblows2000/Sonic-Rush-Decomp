#include <nitro/types.h>

typedef struct {
    u32 cmd;      /* 0x00 */
    u32 unk4;
    u16 id;       /* 0x08 */
    u16 padA;
    u32 unkC;
    u32 arg3;     /* 0x10 */
    u32 arg1;     /* 0x14 */
    u32 size;     /* 0x18 */
    u8 data[0x80];/* 0x1c */
} UnkStruct_0201521c;

void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void func_0201559c(UnkStruct_0201521c *p);

void func_0201521c(u16 id, u32 arg1, const void *src, u32 arg3)
{
    UnkStruct_0201521c s;
    s.cmd = 6;
    s.id = id;
    s.arg3 = arg3;
    s.arg1 = arg1;
    s.size = 0x28;
    MIi_CpuCopy32(src, s.data, 0x40);
    func_0201559c(&s);
}
