#include <nitro/types.h>

typedef struct {
    s32 kind;
} UnkState_02051ebc;

extern UnkState_02051ebc *gSysSound;
extern void *gSoundHeap;
BOOL SysSound_IsInit(void);
void SysSound_Init(void);
void func_0201e7cc(s32 id, void *p);
void SysSound_PlaySE(s32 a);

#pragma thumb on
void func_02051ebc(void) {
    if (SysSound_IsInit() == 0)
        SysSound_Init();
    if (gSysSound->kind == 5) {
        func_0201e7cc(1, gSoundHeap);
        gSysSound->kind = 0;
    }
    switch (gSysSound->kind) {
    case 0:
        SysSound_PlaySE(4);
        break;
    case 2:
        SysSound_PlaySE(19);
        break;
    case 1:
        break;
    }
}
#pragma thumb reset
