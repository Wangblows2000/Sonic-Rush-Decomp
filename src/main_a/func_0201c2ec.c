#include <nitro/types.h>

void func_0201b4a8(void *p);
void func_0202061c(void *list, void *obj);
extern u8 data_02083d28[];

typedef struct { u8 unk0[0x24]; u32 unk24; u8 unk28[0x18]; void *unk40; } UnkStruct_0201c2ec;

void func_0201c2ec(UnkStruct_0201c2ec *s) {
    func_0201b4a8(s->unk40);
    func_0202061c(data_02083d28, s);
    s->unk24 &= ~1;
}
