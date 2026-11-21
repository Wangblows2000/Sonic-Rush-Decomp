#include <nitro/types.h>

typedef struct {
    s32 w0;
    u8 pad4[0xc];
    s32 w10;
    s32 w14;
    s32 w18;
    u8 b1c;
    u8 b1d;
    u16 h1e;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern s32 data_ov001_0231f698;
void SpecialStage_Init(s32 a);

void SpecialStage_Start(void) {
    data_ov001_0231f698 = 0;
    SpecialStage_Init(gGameState.b1c);
}
