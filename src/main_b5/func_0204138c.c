#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 mode;
    s32 state;
    u8 padc[0xe];
    u16 unk1a;
} UnkWork_0204138c;

extern u32 data_02087cb8;

void func_0203c350(void);
void func_0203dda4(void);
u32 func_0203e934(void);
void func_0203dbb4(void);

void func_0204138c(UnkWork_0204138c *w) {
    BOOL loop;
    BOOL f;

    if (w->state != 7) {
        w->state = 7;
        data_02087cb8 &= ~0x100;
        loop = TRUE;
        func_0203c350();
        func_0203dda4();
        f = FALSE;
        do {
            switch ((s32)func_0203e934()) {
            case 1:
                func_0203dbb4();
            case 0:
            case 9:
            case 10:
                loop = f;
                break;
            }
        } while (loop);
    }
    w->unk1a = 0;
}
