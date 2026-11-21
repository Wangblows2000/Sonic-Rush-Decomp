#include <nitro/types.h>

typedef struct UnkStruct_02308800 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02308800;

void MapObj8F_State_RoomBoundDone(void);

void MapObj8F_Action_RoomBoundDone(UnkStruct_02308800 *s) {
    s->func = MapObj8F_State_RoomBoundDone;
}
