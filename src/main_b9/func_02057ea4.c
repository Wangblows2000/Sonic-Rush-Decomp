#include <nitro/types.h>

typedef struct {
    s32 id;
    s32 val;
} UnkEnt_02057ea4;

typedef struct {
    s16 idx;
} UnkState_02057ea4;

typedef struct {
    u8 pad0[4];
    u32 flags;
} UnkObj_02057ea4;

extern UnkEnt_02057ea4 data_02076afc[];
extern UnkState_02057ea4 gPlayerDemoId;
extern u16 gTalkDemoId;

void TalkDemo_Create(void);

#pragma thumb on
void func_02057ea4(UnkObj_02057ea4 *o, s32 v) {
    UnkEnt_02057ea4 *t;
    u8 i;

    if (o->flags & 0x800000)
        return;
    if (v < 0) {
        t = (void *)data_02076afc;
        for (i = 0;; i++) {
            if (gPlayerDemoId.idx == t[i].id) {
                gTalkDemoId = t[i].val;
                break;
            }
        }
    } else {
        gTalkDemoId = v;
    }
    TalkDemo_Create();
}
#pragma thumb reset
