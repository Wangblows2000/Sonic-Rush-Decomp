#include <nitro/types.h>

typedef struct { s32 x, y, z; } Vec_02314c84;

typedef struct UnkStruct_02314c84_e {
    char name[0x20];
    Vec_02314c84 a;
    Vec_02314c84 b;
    Vec_02314c84 c;
    u16 unk44;
    u16 unk46;
    u8 pad48[0x58 - 0x48];
} UnkStruct_02314c84_e;

extern Vec_02314c84 data_ov001_0231efb0;
extern Vec_02314c84 data_ov001_0231f004;
extern s32 data_ov001_0231f6a8;
extern UnkStruct_02314c84_e *data_ov001_0231f6a4;

void MI_CpuFill8(void *dest, u8 data, u32 size);
char *func_0202c4a4(char *dst, const char *src, u32 n);
void func_ov001_02314a48(s32 i, s32 a, s32 b, s32 c, s32 d);

s32 func_ov001_02314c84(const char *name) {
    Vec_02314c84 a = data_ov001_0231efb0;
    Vec_02314c84 b = {0, 0, 0};
    s32 n = data_ov001_0231f6a8;
    Vec_02314c84 c = data_ov001_0231f004;
    s32 i;

    for (i = 0; i < n; i++) {
        if (data_ov001_0231f6a4[i].name[0] == 0)
            break;
    }
    if (i >= n)
        return -1;
    MI_CpuFill8(&data_ov001_0231f6a4[i], 0, 0x20);
    func_0202c4a4(data_ov001_0231f6a4[i].name, name, 0x1f);
    data_ov001_0231f6a4[i].a = a;
    data_ov001_0231f6a4[i].b = b;
    data_ov001_0231f6a4[i].c = c;
    func_ov001_02314a48(i, 0x1555, 0x1555, 0x3000, 0x5dc000);
    data_ov001_0231f6a4[i].unk44 = 0x4000;
    data_ov001_0231f6a4[i].unk46 = 0;
    return i;
}
