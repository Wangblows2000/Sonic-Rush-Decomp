#include <nitro/types.h>

typedef struct UnkStruct_022ed15c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x230 - 0x6c];
    u16 unk230;
} UnkStruct_022ed15c;

void BossHelp_Destroy(void);
void NNS_SndPlayerStopSeqAll(s32 a);
void Boss7_State_Unk_ov000_022ed138(void);

void func_ov000_022ed15c(UnkStruct_022ed15c *s) {
    s->unk230 = 0;
    BossHelp_Destroy();
    NNS_SndPlayerStopSeqAll(0);
    s->func = Boss7_State_Unk_ov000_022ed138;
}
