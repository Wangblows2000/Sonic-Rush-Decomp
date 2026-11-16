#include <nitro/types.h>

typedef struct UnkStruct_022f1290 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x434 - 0x6c];
    s32 unk434;
    u16 unk438;
} UnkStruct_022f1290;

extern s32 gBossStatus[];

void BossFPartD8_State_Unk_ov000_022f1264(void);

void BossFPartD8_Unk_ov000_022f1290(UnkStruct_022f1290 *s) {
    s->unk438 = 0;
    s->unk434 = 0;
    gBossStatus[1] = 0x37;
    s->func = BossFPartD8_State_Unk_ov000_022f1264;
}
