#include <nitro/types.h>

typedef struct UnkStruct_02316148 {
    u8 pad[0x1c];
    s32 unk1C;
} UnkStruct_02316148;

extern UnkStruct_02316148 gDisplayMain;
extern UnkStruct_02316148 gDisplaySub;

void StageObj_Destructor(void *p);

void func_ov000_02316148(void *p) {
    gDisplayMain.unk1C = 0;
    gDisplaySub.unk1C = 0;
    StageObj_Destructor(p);
}
