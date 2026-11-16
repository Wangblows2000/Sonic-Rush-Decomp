#include <nitro/types.h>

typedef struct UnkStruct_023088e8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023088e8;

void MapObj8F_State_Unk_ov000_02308810(void);

void func_ov000_023088e8(UnkStruct_023088e8 *s) {
    s->func = MapObj8F_State_Unk_ov000_02308810;
}
