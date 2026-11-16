#include <nitro/types.h>

typedef struct UnkState_0204b314 UnkState_0204b314;

typedef struct {
    const char *name;
    void *(*getMem)(char *path);
    void (*init)(UnkState_0204b314 *s);
    void (*done)(void *job, char *path);
} UnkEntry_0204b314;

typedef struct {
    UnkEntry_0204b314 *entries;
    u32 count;
} UnkSet_0204b314;

struct UnkState_0204b314 {
    s32 state;
    s32 mode;
    char path[0x40];
    u32 unk48;
    u8 unk4c;
    u8 unk4d;
};

extern UnkSet_0204b314 data_020745a8[][3];
extern UnkState_0204b314 data_022c4490;
extern void *data_022c4484;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
char *func_0202c518(char *dst, const char *src);
void *func_02033fbc(const char *path, void *mem);

void func_0204b314(u32 arg, s32 a, s32 b) {
    UnkEntry_0204b314 *e;
    UnkEntry_0204b314 *entries = data_020745a8[a][b].entries;

    e = &entries[3];

    {
        vu16 z = 0;
        MIi_CpuClear16(z, &data_022c4490, 0x60);
    }
    data_022c4490.unk48 = arg;
    data_022c4490.unk4c = a;
    data_022c4490.unk4d = b;
    func_0202c518(data_022c4490.path, entries[3].name);
    e->init(&data_022c4490);
    data_022c4484 = func_02033fbc(data_022c4490.path, e->getMem(data_022c4490.path));
}
