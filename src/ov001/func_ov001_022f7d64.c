#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
extern u8 *data_ov001_0231f084;
void func_ov001_022f7880(void);
void func_ov001_022f7b94(void);
void func_ov001_022f7790(void);
void Sound_PlaySeqArc(void *a, s32 b, s32 c);

void func_ov001_022f7d64(void) {
    if (*(u32 *)(data_ov001_0231f084 + 0xe38) & 3) {
        gTaskCurrent->fn = func_ov001_022f7880;
        func_ov001_022f7880();
        *(u32 *)(data_ov001_0231f084 + 0xe38) |= 0x4000;
        return;
    }
    *(s32 *)(data_ov001_0231f084 + 0xe90) -= 1;
    if (*(s32 *)(data_ov001_0231f084 + 0xe90) <= 0) {
        gTaskCurrent->fn = func_ov001_022f7b94;
        *(s32 *)(data_ov001_0231f084 + 0xe90) = 0x2d;
        Sound_PlaySeqArc(data_ov001_0231f084 + 0xea0, 8, 1);
    }
    func_ov001_022f7790();
}
