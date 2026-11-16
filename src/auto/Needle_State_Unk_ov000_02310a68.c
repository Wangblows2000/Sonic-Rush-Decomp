/* paramclone of EnemyPawnGroup_State_Unk_ov000_022fdef0 (src/ov000/EnemyPawnGroup_State_Unk_ov000_022fdef0.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct UnkStruct_02310a68 {
    u8 pad[0x8];
    s32 unkC;
} UnkStruct_02310a68;

void Needle_Action_Unk_ov000_023109f4(UnkStruct_02310a68 *s);

void Needle_State_Unk_ov000_02310a68(UnkStruct_02310a68 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    Needle_Action_Unk_ov000_023109f4(s);
}
