#include <nitro/types.h>

typedef struct UnkStruct_022d8bd0 UnkStruct_022d8bd0;
struct UnkStruct_022d8bd0 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022d8bd0 *s);
};

void Boss3PartCC_State_Unk_ov000_022d8bb8(UnkStruct_022d8bd0 *s);

void Boss3PartCC_State_Unk_ov000_022d8bd0(UnkStruct_022d8bd0 *s) {
    s->func = Boss3PartCC_State_Unk_ov000_022d8bb8;
    s->func(s);
}
