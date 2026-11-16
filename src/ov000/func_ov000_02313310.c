#include <nitro/types.h>

typedef struct UnkStruct_02313310 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02313310;

void GmkRail_State_Unk_ov000_0231301c(void);

void func_ov000_02313310(UnkStruct_02313310 *s) {
    s->func = GmkRail_State_Unk_ov000_0231301c;
}
