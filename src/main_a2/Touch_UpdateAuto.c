#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
    u16 touch;
    u16 validity;
} UnkTP_0203787c;

typedef struct {
    u16 x;
    u16 y;
} UnkXY_0203787c;

typedef struct {
    u16 x;
    u16 y;
    u16 lastX;
    u16 lastY;
    u16 startX;
    u16 startY;
    u16 endX;
    u16 endY;
    u32 flags;
    u8 count;
    u8 unk15;
    UnkXY_0203787c hist[4];
} UnkTPState_0203787c;

extern UnkTP_0203787c gTouchSampleBuffer[];
extern UnkTPState_0203787c gTouchState;

u32 func_0200d544(void);
void func_0200d1a4(UnkTP_0203787c *out, const UnkTP_0203787c *raw);
void Touch_FilterAutoSamples(s32 idx);
void MIi_CpuClear16(u16 data, void *dst, u32 size);

void Touch_UpdateAuto(void) {
    UnkTPState_0203787c *st = &gTouchState;
    u32 idx = (u8)func_0200d544();
    UnkTP_0203787c raw;
    UnkTP_0203787c cal;
    u8 i;
    st->lastX = st->x;
    st->lastY = st->y;
    st->flags = (st->flags & ~0x20) | (((st->flags & 0x10) >> 4) << 5);
    raw = gTouchSampleBuffer[idx];
    if (raw.touch == 1) {
        if (st->flags & 0x10) {
            st->flags &= ~0x40;
            Touch_FilterAutoSamples(idx);
            return;
        }
        st->flags &= ~0x80;
        if (raw.validity != 0) {
            return;
        }
        st->flags |= 0x10;
        st->flags |= 0x40;
        func_0200d1a4(&cal, &raw);
        st->startX = cal.x;
        st->x = st->startX;
        st->startY = cal.y;
        st->y = st->startY;
        st->endX = 0xffff;
        st->endY = 0xffff;
        for (i = 0; i < st->count; i++) {
            st->hist[i].x = cal.x;
            st->hist[i].y = cal.y;
        }
    } else {
        st->flags &= ~0x100;
        if (st->flags & 0x10) {
            st->flags &= ~0x10;
            st->flags &= ~0x40;
            st->flags |= 0x80;
            st->endX = st->x;
            st->endY = st->y;
            st->x = 0xffff;
            st->y = 0xffff;
            {
                vu16 fill = 0xffff;
                MIi_CpuClear16(fill, st->hist, sizeof(st->hist));
            }
        } else {
            st->flags &= ~0x80;
            {
                vu16 fill = 0xffff;
                MIi_CpuClear16(fill, st->hist, sizeof(st->hist));
            }
        }
    }
}
