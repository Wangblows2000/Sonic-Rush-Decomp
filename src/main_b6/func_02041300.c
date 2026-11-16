#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[8];
    s32 unkc;
} UnkWork_02041300;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02041300 *work;
} UnkTask_02041300;

extern u32 data_02087cb8;
extern UnkTask_02041300 *data_022c4364;

void func_0203dda4(void);
s32 func_0203e934(void);
void func_0203dbb4(void);
void func_0204138c(void);

void func_02041300(void) {
    BOOL loop = TRUE;

    data_02087cb8 &= 0x100;
    func_0203dda4();
    do {
        switch (func_0203e934()) {
        case 1:
            func_0203dbb4();
        case 9:
        case 10:
            loop = FALSE;
            break;
        }
    } while (loop);
    if (data_022c4364 != NULL) {
        UnkWork_02041300 *w = data_022c4364->work;
        w->unkc = 2;
        w->fn = func_0204138c;
    }
}
