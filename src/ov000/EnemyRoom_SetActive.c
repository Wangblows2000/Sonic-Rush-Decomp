#include <nitro/types.h>

typedef struct UnkStruct_02342348 {
    u8 pad[0x10c];
    u8 unk10C;
} UnkStruct_02342348;

extern UnkStruct_02342348 gCamera;

void EnemyRoom_SetActive(u8 v) {
    gCamera.unk10C = v;
}
