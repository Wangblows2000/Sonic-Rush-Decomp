#include <nitro/types.h>

typedef struct {
    u16 val;
    u16 flags;
} UnkStruct_022501a0;

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[4];
    s32 unkc;
    u8 pad10[4];
    u16 unk14;
    u8 pad16[2];
    u16 cnt;
    u16 unk1a;
} UnkWork_02041698;

extern UnkStruct_022501a0 data_022501a0;

u32 func_0203e934(void);
s32 func_0203c340(void);
void func_0203de84(void *p, u32 size, void (*fn)(void));
void func_0203de90(void (*fn)(void));
void func_02042360(void);
void func_02041454(void);
void func_0204138c(void);

void func_02041698(UnkWork_02041698 *w) {
    UnkStruct_022501a0 *p = &data_022501a0;

    switch (func_0203e934()) {
    case 8:
    case 9:
    case 10:
        w->unkc = 2;
        w->fn = func_0204138c;
        break;
    case 5:
        if (!(p->flags & 2))
            return;
        w->cnt = 4;
        w->unk1a = 0;
        w->fn = func_02041454;
        break;
    case 4:
        if (func_0203c340() != 2)
            return;
        if (!(p->flags & 1))
            return;
        func_0203de84(p, 4, func_02042360);
        func_0203de90(NULL);
        w->cnt = 4;
        w->unk1a = 0;
        w->fn = func_02041454;
        break;
    }
}
