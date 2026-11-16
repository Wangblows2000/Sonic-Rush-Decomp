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

extern UnkSet_0204b314 gStageZoneFileListTable[][3];
extern UnkState_0204b314 gStageLoader;
extern void *gZoneSoundData;

void MIi_CpuClear16(u16 data, void *dest, u32 size);
char *strcpy(char *dst, const char *src);
void *File_Load(const char *path, void *mem);

void func_0204b314(u32 arg, s32 a, s32 b) {
    UnkEntry_0204b314 *e;
    UnkEntry_0204b314 *entries = gStageZoneFileListTable[a][b].entries;

    e = &entries[3];

    {
        vu16 z = 0;
        MIi_CpuClear16(z, &gStageLoader, 0x60);
    }
    gStageLoader.unk48 = arg;
    gStageLoader.unk4c = a;
    gStageLoader.unk4d = b;
    strcpy(gStageLoader.path, entries[3].name);
    e->init(&gStageLoader);
    gZoneSoundData = File_Load(gStageLoader.path, e->getMem(gStageLoader.path));
}
