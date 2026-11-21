#include <nitro/types.h>

typedef struct { u8 pad[0x38]; void (*init)(void *p); void (*fn)(void *p); } UnkStruct_023172cc;
extern UnkStruct_023172cc gSpecialStageGimmickTypeTable[];
void SpecialStageActObj_SpawnAt(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void SpecialStageGimmick_Destroy(void *p);

void SpecialStageGimmick_OnAttacked(u8 *p) {
    UnkStruct_023172cc *e;
    *(s16 *)(p + 0x5c) -= 1;
    if (*(s16 *)(p + 0x5c) > 0) {
        SpecialStageActObj_SpawnAt(0, *(s32 *)(p + 0x20), *(s32 *)(p + 0x24), *(s32 *)(p + 0x28));
        *(s32 *)(p + 0x74) = 0x1e;
        Sound_PlaySeqArc(0, 0x10, 0x18);
        return;
    }
    e = &gSpecialStageGimmickTypeTable[*(s16 *)(p + 0x68)];
    if (e->init != NULL) {
        e->init(p);
    }
    if (e->fn != NULL) {
        *(void (**)(void *))(p + 0xcc) = e->fn;
        return;
    }
    SpecialStageGimmick_Destroy(p);
}
