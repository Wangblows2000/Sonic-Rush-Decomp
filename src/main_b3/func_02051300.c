#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
extern s32 data_022c4640;
extern u16 data_022c4644[];
extern UnkStruct_0207ad9c data_0207ad9c;

s32 func_02042958(void);
u8 *func_0204324c(s32 a);
u8 *func_020432e0(s32 a);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

#pragma thumb on
s32 func_02051300(u16 *dst, s32 len) {
    u16 *src;

    if (data_022c4640) {
        src = data_022c4644;
    } else if (func_02042958()) {
        if (data_0207ad9c.unk0 == 0) {
            switch (data_0207ad9c.unk2) {
            case 0:
            case 1:
                src = (u16 *)(func_0204324c(1) + 6);
                break;
            case 2:
            case 3:
                src = (u16 *)(func_020432e0(0) + 0x50);
                break;
            default:
                src = data_022c4644;
                break;
            }
        } else {
            src = data_022c4644;
        }
    } else {
        src = data_022c4644;
    }
    if (len > 10)
        len = 10;
    MIi_CpuCopy16(src, dst, len * 2);
    if (len < src[10])
        return len;
    return src[10];
}
#pragma thumb reset
