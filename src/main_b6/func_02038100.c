#include <nitro/types.h>

typedef struct {
    u8 pad0[0xfc];
    u32 flags;
} UnkG3_02038100;

extern u32 data_02084d3c[];
extern u32 data_02084d84[];
extern u32 data_02084df4[];
extern UnkG3_02038100 data_02084d38;

void func_02025268(u32 op, const u32 *args, u32 n);

void func_02038100(void) {
    u32 v;

    func_02025268(0x1610, data_02084d3c, 0x11);
    func_02025268(0x19, data_02084d84, 0xc);
    func_02025268(0x1b19, data_02084df4, 0xf);
    v = 2;
    func_02025268(0x10, &v, 1);
    func_02025268(0x15, NULL, 0);
    data_02084d38.flags |= 1;
    data_02084d38.flags &= ~2;
}
