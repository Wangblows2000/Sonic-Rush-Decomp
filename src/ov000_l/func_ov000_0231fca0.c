#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 pad1D[0x24 - 0x1d];
    u32 unk24;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

void func_ov000_0231fe0c(void);

void func_ov000_0231fca0(void) {
    if (data_022c4560.unk1C != 6)
        return;
    if (data_022c4560.unk24 & 0x7f)
        return;
    if (!(Rand() & 1))
        return;
    func_ov000_0231fe0c();
}
