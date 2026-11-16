#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_0230ecf8_1e0;

typedef struct {
    u8 pad0[0x1e0];
    UnkStruct_0230ecf8_1e0 *unk1E0;
} UnkStruct_0230ecf8_10;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0230ecf8_10 *unk10;
} UnkStruct_0230ecf8;

extern u8 data_ov000_0236a04c[];
extern u8 data_ov000_0236a044[];
extern u8 data_ov000_0236a03c[];

void ObjData_Release(void *a);
void MapObject_Destructor(UnkStruct_0230ecf8 *p);

void JumpPedestal_Destructor(UnkStruct_0230ecf8 *p) {
    switch ((u8)(p->unk10->unk1E0->unk4 & 3)) {
    default:
        ObjData_Release(data_ov000_0236a04c);
        break;
    case 1:
        ObjData_Release(data_ov000_0236a044);
        break;
    case 2:
        ObjData_Release(data_ov000_0236a03c);
        break;
    }
    MapObject_Destructor(p);
}
