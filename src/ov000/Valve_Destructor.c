#include <nitro/types.h>

typedef struct UnkStruct_0231af54 {
    u8 pad[0x10];
    void *unk10;
} UnkStruct_0231af54;

void func_ov000_0231a9b0(void *p);
void MapObject_Destructor(UnkStruct_0231af54 *s);

void Valve_Destructor(UnkStruct_0231af54 *s) {
    func_ov000_0231a9b0(s->unk10);
    MapObject_Destructor(s);
}
