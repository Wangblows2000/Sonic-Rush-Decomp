#include <nitro/types.h>

typedef struct { u8 pad[0x88]; void *res[2]; } UnkStruct_022cf90c;
extern void *gSegaLogoFileList[];
void *File_Load(void *p, s32 v);

#pragma thumb on
void SegaLogo_LoadAssets(UnkStruct_022cf90c *p) {
    s32 i;
    for (i = 0; i < 2; i++) {
        p->res[i] = File_Load(gSegaLogoFileList[i], 0);
    }
}
#pragma thumb reset
