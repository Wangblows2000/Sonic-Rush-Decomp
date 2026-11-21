#include <nitro/types.h>

typedef struct UnkStruct_023270d8 {
    u8 unk0;
    u8 pad[0x324];
    u8 unk325;
} UnkStruct_023270d8;

u16 Player_IsPlayer2(UnkStruct_023270d8 *s) {
    if (s->unk0 != 1)
        return 0;
    return s->unk325 != 0;
}
