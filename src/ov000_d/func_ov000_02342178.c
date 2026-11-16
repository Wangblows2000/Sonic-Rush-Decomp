#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    u32 unk14;
} UnkStruct_02342178;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_02342178_4560;

typedef void (*UnkFunc_02342178)(UnkStruct_02342178 *r, s32 a, s32 b);

extern UnkStruct_02342178_4560 gGameState;
extern UnkFunc_02342178 data_ov000_02368a30[][3];
extern u16 data_ov000_0236aab0;

void MIi_CpuClear32(u32 data, void *dest, u32 size);

void func_ov000_02342178(UnkStruct_02342178 *r, s32 a, s32 b) {
    if (r != NULL && data_ov000_02368a30[gGameState.unk1C][gGameState.unk1D] != NULL) {
        {
            vu32 zero = 0;
            MIi_CpuClear32(zero, (void *)0x6008000, 0x8000);
        }
        {
            vu32 zero = 0;
            MIi_CpuClear32(zero, (void *)0x6208000, 0x8000);
        }
        {
            vu32 zero = 0;
            MIi_CpuClear32(zero, (void *)0x6004000, 0x2000);
        }
        {
            vu32 zero = 0;
            MIi_CpuClear32(zero, (void *)0x6204000, 0x2000);
        }
        data_ov000_02368a30[gGameState.unk1C][gGameState.unk1D](r, a, b);
        data_ov000_0236aab0 |= 1;
        *(vu16 *)0x5000000 = 0;
        *(vu16 *)0x5000400 = 0;
        switch (r->unk14 & 0xf) {
        case 1:
            data_ov000_0236aab0 |= 2;
            break;
        case 2:
            data_ov000_0236aab0 &= ~2;
            break;
        }
    } else {
        data_ov000_0236aab0 &= ~1;
    }
}
