#include <nitro/types.h>

typedef struct UnkStruct_022f6654 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_022f6654;

void BossFPartDE_TrackPlayerX(UnkStruct_022f6654 *s);
void BossFPartDE_State_StartAim(void);

void BossFPartDE_State_Track(UnkStruct_022f6654 *s) {
    BossFPartDE_TrackPlayerX(s);
    if (s->unkC++ > 0x1e)
        s->func = BossFPartDE_State_StartAim;
}
