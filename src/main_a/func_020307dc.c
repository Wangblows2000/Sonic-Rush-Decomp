#include <nitro/types.h>

typedef struct { u8 unk0[0x14]; void (*unk14)(void *); } UnkStruct_020307dc;

void func_020307dc(UnkStruct_020307dc *s) {
    if (s->unk14 == NULL) return;
    s->unk14(s);
}
