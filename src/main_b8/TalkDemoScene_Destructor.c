#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02062c4c;

extern void *data_022c4da8;

void TalkDemo_ResetBg1Scroll(void *f);
BOOL TalkDemo_ResetBg2Affine(void *o);
void TalkDemo_DestructorDummy(void);
void TalkDemoScene_ReleaseSprites(void *w);

#define REG_DISPCNT_ (*(vu32 *)0x04000000)

#pragma thumb on
void TalkDemoScene_Destructor(UnkTask_02062c4c *t) {
    void *w = t->work;

    TalkDemo_ResetBg1Scroll(NULL);
    TalkDemo_ResetBg2Affine(NULL);
    {
        u32 plane = (REG_DISPCNT_ & 0x1f00) >> 8;
        REG_DISPCNT_ = (REG_DISPCNT_ & ~0x1f00) | ((plane | 4) << 8);
    }
    TalkDemo_DestructorDummy();
    TalkDemoScene_ReleaseSprites(w);
    data_022c4da8 = NULL;
}
#pragma thumb reset
