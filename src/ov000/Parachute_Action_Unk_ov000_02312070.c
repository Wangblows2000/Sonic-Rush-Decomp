#include <nitro/types.h>

typedef struct UnkStruct_02312070 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02312070;

void Parachute_State_Unk_ov000_0231206c(void);

void Parachute_Action_Unk_ov000_02312070(UnkStruct_02312070 *s) {
    s->func = Parachute_State_Unk_ov000_0231206c;
}
