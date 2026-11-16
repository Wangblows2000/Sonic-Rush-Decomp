#include <nitro/types.h>

typedef struct UnkStruct_0230bff8 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x54 - 0x10];
    u32 unk54;
} UnkStruct_0230bff8;

extern s32 data_ov000_02369f6c;

void MapObj13Part_State_Unk_ov000_0230bff8(UnkStruct_0230bff8 *s) {
    if (s->unkC == 0) {
        s->unk54 |= 4;
        data_ov000_02369f6c = 0;
    }
    s->unkC = 0;
}
