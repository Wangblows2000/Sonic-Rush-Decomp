#include <nitro/types.h>

extern u8 *data_ov001_0231f5a8;
void *Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
u8 *ExtraBoss_GetWork(void);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void func_ov001_023059b4(void);

void func_ov001_02305aec(void) {
    s32 i;
    *(void **)(data_ov001_0231f5a8 + 0x14) = Task_Create(func_ov001_023059b4, 0, 0, 0x3100, 2, 0);
    *(u16 *)(data_ov001_0231f5a8 + 0x5e6) = 0;
    for (i = 4; i <= 7; i++) {
        *(s16 *)(data_ov001_0231f5a8 + i * 0x48 + 0x9c) = 0x80;
        *(s16 *)(data_ov001_0231f5a8 + i * 0x48 + 0x9e) = 0x5b;
    }
    if (*(s32 *)(ExtraBoss_GetWork() + 0x3c) == 0) {
        Sound_PlayVoice(0, 0, 0x22);
    } else {
        Sound_PlayVoice(0, 0, 0x2b);
    }
}
