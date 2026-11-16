#include <nitro/types.h>

extern vu32 data_0207f088;
extern vu32 data_0207f08c;

extern BOOL func_0200ea14(void);
extern void func_0200e2d0(u32 data);
extern void func_0200dfa8(u32 a, u32 b, u32 c, u32 d);

u32 func_0200e738(u32 a, u32 b)
{
    if (!func_0200ea14()) {
        return 1;
    }
    data_0207f088 = 0;
    func_0200e2d0(0x03006000);
    while (data_0207f088 == 0) {
    }
    data_0207f088 = 0;
    data_0207f08c = 0;
    func_0200dfa8(0, 2, 0, 0);
    func_0200e2d0((a & 0xff) | 0x02006100);
    func_0200e2d0((b & 0xffff) | 0x01010000);
    return 0;
}
