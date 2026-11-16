#include <nitro/types.h>

typedef struct { u8 pad[0x10]; s32 *work; } UnkStruct_022c6944;
UnkStruct_022c6944 *Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void EndingBg0Picture_Main(void);

void Ending_CreateBg0PictureTask(u8 *p) {
    s32 *w;
    *(s32 *)(p + 0x69c) = *(s32 *)(p + 0x698);
    *(s32 *)(p + 0x698) += 1;
    w = Task_Create(EndingBg0Picture_Main, 0, 0, 0xf300, 0x1173, 8)->work;
    w[0] = *(s32 *)(p + 0x698);
    w[1] = (s32)p;
}
