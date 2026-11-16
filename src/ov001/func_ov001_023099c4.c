#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

void func_ov001_023099c4(void) {
    *(s32 *)(data_ov001_0231f5bc + 0xbc8) = 0;
    Sound_PlaySeqArc(0, 0x10, 1);
}
