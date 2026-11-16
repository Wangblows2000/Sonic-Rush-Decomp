#include <nitro/types.h>

typedef struct UnkStruct_02306ad0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02306ad0;

void CraneSlide_State_Unk_ov000_023064d8(void);

void CraneSlide_Action_Unk_ov000_02306ad0(UnkStruct_02306ad0 *s) {
    s->func = CraneSlide_State_Unk_ov000_023064d8;
}
