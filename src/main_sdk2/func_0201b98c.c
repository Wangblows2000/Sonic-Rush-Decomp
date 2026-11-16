#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 fader[0x10];
    u8 status;
} UnkSndPlayer_0201b98c;

void func_0201b7a4(UnkSndPlayer_0201b98c *p);
void func_020205a0(void *fader, s32 target, s32 frames);
void func_0201b5d8(UnkSndPlayer_0201b98c *p, u8 prio);

void func_0201b98c(UnkSndPlayer_0201b98c *p, s32 frames)
{
    if (p == NULL) {
        return;
    }
    if (p->status == 0) {
        return;
    }
    if (frames == 0) {
        func_0201b7a4(p);
        return;
    }
    func_020205a0(p->fader, 0, frames);
    func_0201b5d8(p, 0);
    p->status = 2;
}
