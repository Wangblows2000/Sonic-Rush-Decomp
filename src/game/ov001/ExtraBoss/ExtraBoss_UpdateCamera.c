/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_GetWork();
s32 ExtraBoss_CameraFollowPlayer();
s32 ExtraBoss_CameraTrackObject();

void ExtraBoss_UpdateCamera(void) {
    s32 t0 = ExtraBoss_GetWork();
    s32 t1 = *(u32 *)((u8 *)t0 + 0x318);
    if (t1 == 0) {
        ExtraBoss_CameraFollowPlayer(t1);
        return;
    }
    ExtraBoss_CameraTrackObject();
}
