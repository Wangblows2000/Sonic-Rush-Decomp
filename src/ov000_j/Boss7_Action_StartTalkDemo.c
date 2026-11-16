#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
} UnkStruct_022edc24;

extern s32 gGameState;
extern u16 gTalkDemoId;

void TalkDemo_Create(void);
void Boss7_State_Unk_ov000_022edbec(UnkStruct_022edc24 *p);

void Boss7_Action_StartTalkDemo(UnkStruct_022edc24 *p) {
    if (gGameState == 0)
        gTalkDemoId = 0xa;
    else
        gTalkDemoId = 0xb;
    TalkDemo_Create();
    p->unk68 = Boss7_State_Unk_ov000_022edbec;
}
