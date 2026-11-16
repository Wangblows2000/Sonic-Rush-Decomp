#include <nitro/types.h>

void Sound_PlayVoice(s32 a, s32 b, s32 c);
void func_ov001_022f40b0(void);

void func_ov001_022f4148(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    *(u32 *)(p + 0x14) ^= 0x30000;
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) == 0) {
        Sound_PlayVoice(0, 0, 0x23);
    }
    if (q[0x10c] & 2) {
        *(void (**)(void))(p + 0x74) = func_ov001_022f40b0;
        *(s32 *)(p + 0x1c8) = 0xfd;
    }
}
