#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u8 *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void AnimatorSprite_Draw(void *p);
void func_02044c78(void) { AnimatorSprite_Draw(gTaskCurrent->unk10 + 0x448); }
