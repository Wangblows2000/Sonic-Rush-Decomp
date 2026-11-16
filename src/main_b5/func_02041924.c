#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[4];
    s32 unkc;
    u8 pad10[0xa];
    u16 unk1a;
} UnkWork_02041924;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
} UnkTask_02041924;

extern u32 data_02087cb8;
extern u16 *data_0224ff44;
extern UnkTask_02041924 *data_022b4574;

u32 func_0203e934(void);
void func_0204138c(void);
void func_02042684(void);
void func_02041904(void);

void func_02041924(UnkWork_02041924 *w) {
    switch (func_0203e934()) {
    case 8:
    case 9:
    case 10:
        w->unkc = 2;
        w->fn = func_0204138c;
        break;
    case 5:
        data_02087cb8 |= 0x200;
        if (data_0224ff44[1] & 1) {
            data_022b4574->fn = func_02042684;
            w->fn = func_02041904;
        }
        w->unk1a = 0;
        break;
    }
}
