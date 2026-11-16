#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 flags;
    u16 unkc;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkStruct_022c46c0;

typedef struct {
    s32 timer;
    s32 arg;
} UnkWork_022cf324;

typedef struct {
    u8 pad[0x10];
    UnkWork_022cf324 *work;
} UnkTask_022cf324;

extern UnkStruct_022c4680 data_022c4680;
extern UnkStruct_022c46c0 data_022c46c0;

void func_020522f4(void);
void func_ov001_022ceef4(void);
UnkTask_022cf324 *func_020372d0(void (*fn)(void), void *a, s32 b, s32 c, s32 d, s32 size);
void func_ov001_022cf190(void);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_020521b4(void);
void func_ov001_022cf2d4(s32 arg);

#pragma thumb on
void func_ov001_022cf324(s32 arg) {
    if ((data_022c4680.unkc >= 0x1a || data_022c46c0.unk24 >= 0x1c) && !(data_022c4680.flags & 0x200)) {
        UnkWork_022cf324 *w;
        func_020522f4();
        func_ov001_022ceef4();
        w = func_020372d0(func_ov001_022cf190, NULL, 0, 0x1000, 0, sizeof(UnkWork_022cf324))->work;
        w->timer = 0;
        w->arg = arg;
        func_02050078(0x15, 3, 5, 0, 0, 0, 0, 0x200010);
        return;
    }
    func_020521b4();
    func_ov001_022cf2d4(arg);
}
#pragma thumb reset
