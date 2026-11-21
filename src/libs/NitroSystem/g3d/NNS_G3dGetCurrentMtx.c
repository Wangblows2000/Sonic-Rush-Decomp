#include <nitro/types.h>

void NNS_G3dGeFlushBuffer(void);
BOOL G3X_GetClipMtx(void *m);
void MTX_Copy44To43_(void *m, void *dst);
BOOL G3X_GetVectorMtx(void *p);

#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_PUSH (*(vu32 *)0x04000444)
#define REG_MTX_POP (*(vu32 *)0x04000448)
#define REG_MTX_IDENTITY (*(vu32 *)0x04000454)

void NNS_G3dGetCurrentMtx(void *mtx, void *p) {
    u8 buf[0x40];
    NNS_G3dGeFlushBuffer();
    REG_MTX_MODE = 0;
    REG_MTX_PUSH = 0;
    REG_MTX_IDENTITY = 0;
    if (mtx != NULL) {
        while (G3X_GetClipMtx(buf)) {
        }
        MTX_Copy44To43_(buf, mtx);
    }
    if (p != NULL) {
        while (G3X_GetVectorMtx(p)) {
        }
    }
    REG_MTX_POP = 1;
    REG_MTX_MODE = 2;
}
