#include <nitro/types.h>

typedef struct UnkStruct_023088e8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023088e8;

void MapObj8F_State_SetRoomBound(void);

void MapObj8F_Action_InitRoomBound(UnkStruct_023088e8 *s) {
    s->func = MapObj8F_State_SetRoomBound;
}
