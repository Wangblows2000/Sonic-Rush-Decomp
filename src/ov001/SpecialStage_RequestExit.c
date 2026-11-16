#include <nitro/types.h>

typedef struct { u8 pad[0xc]; u8 *p; u8 pad2[0x14]; s32 v; } UnkStruct_0230c0e0;
extern UnkStruct_0230c0e0 *gSpecialStageWork;
void SpecialStage_State_Exit(void);

void SpecialStage_RequestExit(s32 v) {
    UnkStruct_0230c0e0 *t = gSpecialStageWork;
    t->v = v;
    *(void (**)(void))(t->p + 8) = SpecialStage_State_Exit;
}
