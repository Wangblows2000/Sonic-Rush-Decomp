#include <nitro/types.h>

extern u32 data_02087cb8;
extern void *data_022c4364;
extern u8 data_0224ffa0[];
extern u8 data_022501a0[];

BOOL func_02010b54(void);
void func_0203de90(void (*fn)(void));
u32 func_0203e934(void);
void func_0203dbb4(void);
void func_0203c350(void);
void func_0203dda4(void);
void func_020371c8(void *p);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

static inline void Clear32_02042d34(void *dest, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, dest, size);
}

void func_02042d34(s32 mode) {
    BOOL loop;
    u32 cnt;

    loop = TRUE;
    switch (mode) {
    case 0:
        cnt = 0;
        data_02087cb8 &= ~0x100;
        func_0203c350();
        if (!func_02010b54())
            func_0203de90(NULL);
        do {
            switch (func_0203e934()) {
            case 1:
                func_0203dbb4();
                break;
            case 0:
                loop = FALSE;
                break;
            case 3:
                break;
            case 9:
            case 10:
                if (++cnt > 3)
                    loop = FALSE;
                else
                    func_0203dda4();
                break;
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            default:
                func_0203dda4();
                break;
            }
        } while (loop);
        Clear32_02042d34(data_0224ffa0, 0x200);
        Clear32_02042d34(data_022501a0, 0x240);
        data_02087cb8 |= 0x200;
        data_02087cb8 &= ~0x100;
        break;
    case 1:
        if (!func_02010b54())
            func_0203de90(NULL);
        break;
    }
    if (data_022c4364 == NULL)
        return;
    func_020371c8(data_022c4364);
    data_022c4364 = NULL;
}
