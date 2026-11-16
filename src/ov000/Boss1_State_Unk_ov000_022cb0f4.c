#include <nitro/types.h>

typedef struct UnkStruct_022cb0f4 {
    u8 pad[0x220];
    u16 unk220;
} UnkStruct_022cb0f4;

void func_ov000_022cb344(void);

void Boss1_State_Unk_ov000_022cb0f4(UnkStruct_022cb0f4 *s) {
    if (s->unk220 != 0) {
        s->unk220--;
        return;
    }
    func_ov000_022cb344();
}
