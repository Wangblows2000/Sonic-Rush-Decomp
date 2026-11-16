#include <nitro/types.h>

typedef struct UnkStruct_02303e24 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02303e24;

void MapObj73_State_Unk_ov000_02303dec(void);

void func_ov000_02303e24(UnkStruct_02303e24 *s) {
    s->func = MapObj73_State_Unk_ov000_02303dec;
}
