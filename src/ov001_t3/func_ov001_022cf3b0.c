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
    s32 unk0;
    s32 unk4;
} UnkWork_022cf3b0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cf3b0 *work;
} UnkTask_022cf3b0;

extern UnkStruct_022c4680 data_022c4680;
extern UnkStruct_022c46c0 data_022c46c0;

void func_020522f4(void);
void func_ov001_022ceef4(void);
UnkTask_022cf3b0 *func_020372d0(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void func_ov001_022cf21c(void);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_020521b4(void);
void func_ov001_022cf324(s32 a);

#pragma thumb on
void func_ov001_022cf3b0(s32 a) {
    UnkWork_022cf3b0 *w;

    if ((data_022c4680.unkc >= 0x1a || data_022c46c0.unk24 >= 0x1c) && (data_022c4680.flags & 0x100) == 0) {
        func_020522f4();
        func_ov001_022ceef4();
        w = func_020372d0(func_ov001_022cf21c, NULL, 0, 0x1000, 0, sizeof(UnkWork_022cf3b0))->work;
        w->unk0 = 0;
        w->unk4 = a;
        func_02050078(0x14, 3, 5, 0, 0, 0, 0, 0x200010);
        return;
    }
    func_020521b4();
    func_ov001_022cf324(a);
}
#pragma thumb reset
