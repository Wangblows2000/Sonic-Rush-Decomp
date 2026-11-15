#include <nitro/types.h>

typedef struct UnkNode_02036fdc UnkNode_02036fdc;
struct UnkNode_02036fdc { u32 unk0; UnkNode_02036fdc *next; u8 unk8[0x12]; u16 unk1A; };
extern UnkNode_02036fdc *data_022b457c;
extern UnkNode_02036fdc *data_022b4588;
void func_020371c8(UnkNode_02036fdc *n);

void func_02036fdc(u16 id) {
    UnkNode_02036fdc *n;
    for (n = data_022b457c->next; n != data_022b4588; n = n->next) {
        if (id == n->unk1A) {
            func_020371c8(n);
        }
    }
}
