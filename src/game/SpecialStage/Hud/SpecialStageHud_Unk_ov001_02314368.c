#include <nitro/types.h>

typedef struct { u8 pad[0x10]; u16 a; u16 b; } UnkStruct_02314368_b;
typedef struct { u8 pad[0x14]; UnkStruct_02314368_b *p; } UnkStruct_02314368;
extern UnkStruct_02314368 *gSpecialStageData;

u16 SpecialStageHud_Unk_ov001_02314368(void) {
    return gSpecialStageData->p->b;
}
