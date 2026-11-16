#include <nitro/types.h>

typedef struct UnkStruct_0230be10 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x54 - 0x10];
    u32 unk54;
} UnkStruct_0230be10;

extern s32 data_ov000_02369f70;

void MapObj13Part_State_Unk_ov000_0230be10(UnkStruct_0230be10 *s) {
    if (s->unkC == 0) {
        s->unk54 |= 4;
        data_ov000_02369f70 = 0;
    }
    s->unkC = 0;
}
