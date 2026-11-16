#include <nitro/types.h>

typedef struct UnkStruct_022ebfac {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0xa];
    void (*func)(void);
} UnkStruct_022ebfac;

void Boss7PartD7_State_Unk_ov000_022ebfa8(void);

void Boss7PartD7_State_Unk_ov000_022ebfac(UnkStruct_022ebfac *s) {
    s->unk5C |= 0x20;
    s->func = Boss7PartD7_State_Unk_ov000_022ebfa8;
}
