#include <nitro/types.h>

void func_ov000_02355b44(u8 *p);
void Sound_PlaySeqArcEx(void *p, s32 v, s32 n);

void func_ov000_02355afc(u8 *p) {
    func_ov000_02355b44(p);
    Sound_PlaySeqArcEx(p + 0x384, 0, 0x72);
}
